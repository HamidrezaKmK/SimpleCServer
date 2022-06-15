#include "multiThreadServer.h"
#include "request.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <fcntl.h>

int free_worker = 0;
int WORKER_COUNT = MAX_WORKERS;

void construct_workers(int n, struct Config *conf) {
	WORKER_COUNT = n;
	for (int i = 0; i < n; i++) {
		all_workers[i].id = i;
		all_workers[i].busy = 0;

		// one config for all
		all_workers[i].config = conf;

		all_workers[i].log = (struct Log *) malloc(sizeof(struct Log));
		all_workers[i].log->initialized = 0;
	}
}

struct Worker* get_free_worker(int socket) {
	for (free_worker = (free_worker + 1) % WORKER_COUNT; all_workers[free_worker].busy; free_worker = (free_worker + 1) % WORKER_COUNT);
	struct Worker *ret = &all_workers[free_worker];
	ret->socket = socket;
	return ret;
}

void *handle_request(void * worker_id) {
	struct Worker *worker = &all_workers[*(int *)worker_id];
	worker->busy = 1;
	clearlog(worker->log);
	settime(worker->log);

	if (worker->socket == -1) {
		add_msg(worker->log, "no connection with client -- connection failed!");
		worker->busy = 0;
		close(worker->socket);
		return (void *) 0;
	}
	add_msg(worker->log, "connection established!");

	// read from request buffer
	char *buf = malloc(worker->config->BUFSIZE);
	memset(buf, 0, sizeof(buf));
	read(worker->socket, buf, worker->config->BUFSIZE);

	if (strlen(buf) == 0) {
		add_msg(worker->log, "the sent request is empty!");
		worker->busy = 0;
		close(worker->socket);
		return (void *) 0;
	}

	struct Request * req = parse_request(buf, worker->config);

	add_msg(worker->log, "connection accepted!");
	set_user_info(worker->log, req->user_info);
	if (req->type == WEBPAGE_REQUEST) {
		add_msg(worker->log, "Webpage request");
		char *webpage = read_webpage(relative_file_address(req->location));
		if (webpage == NULL)
		{
			add_msg(worker->log, "no html file found!");
			webpage = read_webpage(relative_file_address(worker->config->ErrorHTML));
		}
		write(worker->socket, webpage, strlen(webpage) - 1);
	} else if (req->type == IMAGE_REQUEST) {
		add_msg(worker->log, "Image request");
		char *address = relative_file_address(req->location);

		int fdimg = open(address, O_RDONLY);

		if (fdimg == -1){
			add_msg(worker->log, "File not found error!");
		} else {
			sendfile(worker->socket, fdimg, NULL, 200000);
		}
		close(fdimg);
	} else {
		add_msg(worker->log, "Request type is not implemented!");
	}
	add_msg(worker->log, "client connection closed!");
	worker->busy = 0;
	close(worker->socket);
	return (void*) 0;
}

void handle_job(struct Worker * worker) {
	int ret_val = pthread_create(&worker->thread, NULL, &handle_request, (void *) worker->id);
	if (ret_val != 0) {
		printf("Could not build a new thread!\n");
		close(worker->socket);
		return;
	}
}