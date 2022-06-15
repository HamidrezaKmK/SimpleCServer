#ifndef REQUESTS_H_   /* Include guard */
#define REQUESTS_H_

#include "config.h"

#define INNER_BUFFER_SIZE 2000

enum request_type {WEBPAGE_REQUEST, IMAGE_REQUEST};

struct Request {
	char * user_info;
	enum request_type type;
	char * location;
};

struct Request* parse_request(char * buf, struct Config *conf);

#endif