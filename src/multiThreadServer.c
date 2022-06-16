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
#include <pthread.h>

int free_worker = 0;
int WORKER_COUNT = MAX_WORKERS;
struct Worker *all_workers;

void construct_workers(int n, struct Config *conf) {
	WORKER_COUNT = n;
	all_workers = malloc(WORKER_COUNT * sizeof(struct Worker *));
	for (int i = 0; i < n; i++) {
		struct Worker *current_worker = all_workers + i;
		current_worker->id = i;
		current_worker->busy = 0;

		// one config for all
		current_worker->config = conf;
		current_worker->log = (struct Log *) malloc(sizeof(struct Log));
		current_worker->log->initialized = 0;
	}

}

struct Worker* get_free_worker(int socket) {
	int cnt = 0;
	for (; all_workers[free_worker].busy && cnt < WORKER_COUNT; free_worker = (free_worker + 1) % WORKER_COUNT, cnt++);
	
	// if no free workers are available
	if (cnt == WORKER_COUNT)
		return NULL;
	
	struct Worker *ret = all_workers + free_worker;
	ret->socket = socket;
	return ret;
}

void *handle_request(void * worker_id) {
	int worker_index = *(int *)worker_id;
	struct Worker *worker = &all_workers[worker_index];
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

		char *webpage = read_webpage(req->location);
		if (webpage == NULL)
		{
			add_msg(worker->log, "no html file found!");
			webpage = read_webpage(path_join(worker->config->WebContentLocation, worker->config->ErrorHTML));
		}
		write(worker->socket, webpage, strlen(webpage) - 1);
	} else if (req->type == IMAGE_REQUEST) {
		add_msg(worker->log, "Image request");
		
		char *address = req->location;

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
	void * ret_val = pthread_create(&worker->thread, NULL, &handle_request, (void *) &worker->id);
	if (ret_val != 0) {
		printf("Could not build a new thread!\n");
		close(worker->socket);
		return;
	}
}

int get_number_of_busy_workers() {
	int ret = 0;
	for (int i = 0; i < WORKER_COUNT; i++)
		if (all_workers[i].busy)
			ret++;
	return ret;
}

void interrupt_handler(int sig)
{
	//printf("Interupt detected ...\n All Logs\n%s\n", all_logs);
	// TODO: Complete this:

	// FILE * logfile;
	// char *address = relative_file_address(LogFile);
	// printf("\n save logs to: %s\n", address);
	// logfile = fopen(address, "a");
	// if(logfile == NULL)
	// {
	// 	printf("could not open logfile.\n saving logs failed.\n");
	// }
	// else
	// {
	// 	fprintf(logfile, "%s", all_logs);
	// }
	close(fd_server);
	exit(0);
}
