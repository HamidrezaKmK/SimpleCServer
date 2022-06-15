#ifndef MULTI_THREAD_SERVER_H_   /* Include guard */
#define MULTI_THREAD_SERVER_H_

#include <pthread.h>
#include "logger.h"
#include "config.h"

#define MAX_WORKERS 10

struct Worker {
	int id; // The id of the worker
	int busy; // int: 1 or 0 if worker is busy or not
	int socket;
	pthread_t thread;
	struct Log * log;
	struct Config * config;
};

struct Worker all_workers[MAX_WORKERS];

struct Worker* get_free_worker(int socket);  /* An example function declaration */

void handle_job(struct Worker * worker);

void construct_workers(int n, struct Config *conf);

#endif