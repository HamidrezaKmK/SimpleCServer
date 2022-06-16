#ifndef UTILS_H_   /* Include guard */
#define UTILS_H_

#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#ifdef __MAC__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/uio.h>
#endif
#ifdef __LINUX__
    #include <sys/sendfile.h>
#endif

int min(int a, int b);
char *read_webpage(char *webpage_location);
char* path_join(char* content_location, char *location);
void sendfile_wrapper(int my_socket, int fdesc);
#endif