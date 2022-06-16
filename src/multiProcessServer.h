#ifndef MULTI_PROCESS_SERVER_H_   /* Include guard */
#define MULTI_PROCESS_SERVER_H_

#include "logger.h"
#include "config.h"
#include "utils.h"
#include "request.h"
#include "serverUtils.h"

#define MAX_WORKERS 10

enum {READ_PIPE, WRITE_PIPE};

struct Worker {
	int id; // The id of the worker
	int busy; // int: 1 or 0 if worker is busy or not
	int socket;
	struct Log * log;
	struct Config * config;
	pid_t pid;
	int pipes[2];
};


void handle_job(struct Worker * worker);

void interrupt_handler(int sig);

void flush_log(struct Worker *worker);

#endif