#ifndef SERVER_UTILS_H_   /* Include guard */
#define SERVER_UTILS_H_

#ifdef MULTITHREAD
#include "multiThreadServer.h"
#endif

#ifdef MULTIPROC
#include "multiProcessServer.h"
#endif

#include "utils.h"

#define MAX_WORKERS 10

int free_worker;
int WORKER_COUNT;
struct Worker *all_workers;
struct Config *global_config;


int fd_server;

struct Worker* get_free_worker(int socket);  /* An example function declaration */

void construct_workers(int n, struct Config *conf);

int get_number_of_busy_workers();

void *handle_request(void * worker_id);

void save_logs();

#endif