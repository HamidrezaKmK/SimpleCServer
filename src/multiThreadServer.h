#ifndef MULTI_THREAD_SERVER_H_   /* Include guard */
#define MULTI_THREAD_SERVER_H_

#include <pthread.h>
#include "logger.h"
#include "config.h"
#include "utils.h"
#include "request.h"
#include "serverUtils.h"

#define MAX_WORKERS 10

int fd_server;

struct Worker {
	int id; // The id of the worker
	int busy; // int: 1 or 0 if worker is busy or not
	int socket;
	pthread_t thread;
	struct Log * log;
	struct Config * config;
};

void handle_job(struct Worker * worker);

void interrupt_handler(int sig);

void flush_log(struct Worker *worker);

#endif