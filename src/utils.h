#ifndef UTILS_H_   /* Include guard */
#define UTILS_H_

#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

int min(int a, int b);
char *read_webpage(char *webpage_location);
char* path_join(char* content_location, char *location);
void sendfile_wrapper(int my_socket, int fdesc);
#endif