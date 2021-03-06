#include "serverUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

int free_worker = 0;
int WORKER_COUNT = MAX_WORKERS;
struct Worker *all_workers = NULL;
struct Config *global_config = NULL;


/* 
The main handle request function:

given a worker id it uses the worker to handle an HTTP request
after the request is done, the log is added to the end of the all_logs in logger.h
*/

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
        flush_log(worker);
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
        flush_log(worker);
		return (void *) 0;
	}

	struct Request * req = parse_request(buf, worker->config);

	add_msg(worker->log, "Connection accepted!");

	set_user_info(worker->log, req->user_info);
	if (req->type == WEBPAGE_REQUEST) {
		add_msg(worker->log, "Webpage request");
		

		char *webpage = read_webpage(req->location);
		if (webpage == NULL)
		{

			printf("Not found\n");

			add_msg(worker->log, "No html file found!");
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
			sendfile_wrapper(worker->socket, fdimg);
		}
		close(fdimg);
	} else if (req->type == BAD_REQUEST) {
		char *error_page = read_webpage(path_join(worker->config->WebContentLocation, worker->config->ErrorHTML));
		write(worker->socket, error_page, strlen(error_page) - 1);	
	} else {
		add_msg(worker->log, "Request type is not implemented!");
	}
	add_msg(worker->log, "Client connection closed!");
	worker->busy = 0;
	close(worker->socket);
    
    flush_log(worker);
	return (void*) 0;
}


void construct_workers(int n, struct Config *conf) {
	global_config = conf;
	WORKER_COUNT = n;
	all_workers = (struct Worker *) malloc(WORKER_COUNT * sizeof(struct Worker));
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


// The following function returns an integer equal to the number of workers that are currently busy
int get_number_of_busy_workers() {
	int ret = 0;
	for (int i = 0; i < WORKER_COUNT; i++)
		if (all_workers[i].busy)
			ret++;
	return ret;
}

void save_logs() {
	// Append to the logfile specified in the config
	char *address_all_logs = path_join(global_config->EXECUTABLE_LOCATION, global_config->LogFile);
	char *address_new_logs = path_join(global_config->EXECUTABLE_LOCATION, "last-log.txt");
	
	printf("Saving logfiles to %s and %s\n", address_all_logs, address_new_logs);

	FILE *all_log_file = fopen(address_all_logs, "a");
	FILE *new_log_file = fopen(address_new_logs, "w");
	if (all_log_file == NULL || new_log_file == NULL) {
	 	printf("Could not open logfile!\n");
		exit(1);
	}
	
	fprintf(all_log_file, "%s", get_all_logs());
	fprintf(new_log_file, "%s", get_all_logs());
	close(fd_server);
	exit(0);
}
