#include "multiThreadServer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <pthread.h>

// The following function creates a new thread using worker's thread and handles any request 
// that is currently buffered in the worker's socket
void handle_job(struct Worker * worker) {
	void * ret_val = pthread_create(&worker->thread, NULL, &handle_request, (void *) &worker->id);
	if (ret_val != 0) {
		printf("Could not build a new thread!\n");
		close(worker->socket);
		return;
	}
}



void interrupt_handler(int sig)
{	
	printf("Handling interrupt ...\n");
	fflush(stdout);

	// wait for all threads to end and store their log files
	for (int i = 0; i < WORKER_COUNT; i++) {
		struct Worker *current_worker = all_workers + i;
		if (current_worker->busy)
			pthread_join(current_worker->thread, NULL);
	}
	
	save_logs();
}

void flush_log(struct Worker *worker) {
	add_to_log(worker->log);
}
