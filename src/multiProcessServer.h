#ifndef MULTI_PROCESS_SERVER_H_   /* Include guard */
#define MULTI_PROCESS_SERVER_H_

#define MAX_WORKERS 10

int get_free_worker();  /* An example function declaration */

struct Worker {
	int id; // The id of the worker
	int busy; // int: 1 or 0 if worker is busy or not
} all_workers[MAX_WORKERS];

#endif