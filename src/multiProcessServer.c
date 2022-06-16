
#include "multiProcessServer.h"
#include "serverUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>


/* 
The main handle request function:

given a worker id it uses the worker to handle an HTTP request
after the request is done, the log is added to the end of the all_logs in logger.h
*/

void flush_log(struct Worker *worker) {
    char *log_stream = streamify(worker->log);
    write(worker->pipes[WRITE_PIPE], log_stream, strlen(log_stream) + 1);
    close(worker->pipes[WRITE_PIPE]);
}

void handle_job(struct Worker * worker) {
	// TODO
    if (pipe(worker->pipes) != 0) {
        printf("Could not pipe!\n");
        exit(1);
    }
    worker->pid = fork();
    if (worker->pid < 0) {
        printf("Fork unsuccessful!\n");
        exit(1);
    }
    /*
    Each worker has two copies, one is the parent worker and the other is the child worker.
    The child worker only has a write pipe and the parent worker only has a read pipe.
    */
    if (worker->pid == 0) {
        // This is the child process: We should exit when the process is complete!
        close(worker->pipes[READ_PIPE]);
        handle_request((void *) &worker->id);
        exit(0);
    } else {
        // This is the parent process
        close(worker->pipes[WRITE_PIPE]);
        close(worker->socket);
        char buf[worker->config->BUFSIZE];
        read(worker->pipes[READ_PIPE], buf, worker->config->BUFSIZE);
        add_stream_to_log(buf);
    }
}


void interrupt_handler(int sig)
{	
	printf("Handling interrupt ...\n");
	fflush(stdout);
    
    // TODO: kill all the busy workers processes

    save_logs();
}
