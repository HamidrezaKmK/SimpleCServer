
/*\
 * Web server code created by C
 * A lot of multiprocessing jewel hidden in this code!
\*/

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "serverUtils.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


int main(int argc, char *argv[])
{
	printf("Starting server ...\n");
	printf("multithread mode setup ...\n");
	signal(SIGINT, interrupt_handler);
	struct Config *conf = read_config_file("configurations/server.conf");
	printf("Configurations:\n");
	print_configurations(conf);
	// construct 10 workers with the same configuration
	construct_workers(10, conf);
	
	fd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (fd_server == -1)
	{
		printf("socket failed with error\n");
		return 1;
	}
	socklen_t sin_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	sin_len = sizeof(client_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(conf->PORTNUM);

	if (bind(fd_server, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
		printf("bind failed with error");
		close(fd_server);
		return 1;
	}
	
	if (listen(fd_server, 10) == -1) {
		printf("listen failed with error");
		close(fd_server);
		return 1;
	}
	
	while (1) {
		
		printf("Waiting for client ...\n");
		fflush(stdout);
		
		int fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		
		printf("New client connected on socket: %d\n", fd_client);
		printf("Number of busy workers: %d\n", get_number_of_busy_workers());
		fflush(stdout);

		struct Worker *current_worker = get_free_worker(fd_client);
		if (current_worker == NULL) {
			printf("No free workers were available -- try again later!\n");
			continue;
		}
		handle_job(current_worker);
	}

	return 0;
}
