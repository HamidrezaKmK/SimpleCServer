
#include "request.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

struct Request* parse_request(char * buf, struct Config *conf) {
	struct Request *req = (struct Request *) malloc(sizeof(struct Request));

	char *buf_start = strstr(buf, "User-Agent:") + 12;
	char *buf_end = strstr(buf_start, "\n");
	char *user_info = malloc(buf_end - buf_start + 1);
	memcpy(user_info, buf_start, buf_end - buf_start);
    
	user_info[buf_end - buf_start] = '\0';
	req->user_info = user_info;

	char location[INNER_BUFFER_SIZE];
	int start_loc = 5;
	int end_loc = strchr(buf + 5, ' ') - buf;
	memcpy(location, buf + start_loc, end_loc - start_loc);
	location[end_loc - start_loc] = '\0';

	if (start_loc == end_loc) {
        
		strcpy(location, conf->StartingPageName);
		
		start_loc = 0;
		end_loc = strlen(location);
	}

	req->location = path_join(conf->WebContentLocation, location);

	if (!strncmp(location + (end_loc - start_loc - 4), "html", 4)) {
		req->type = WEBPAGE_REQUEST;
	} else if (!strncmp(location + (end_loc - start_loc - 3), "jpg", 3)) {
		req->type = IMAGE_REQUEST;
	} else if (!strncmp(location + (end_loc - start_loc - 4), "jpeg", 4)) {
		req->type = IMAGE_REQUEST;
	} else if (!strncmp(location + (end_loc - start_loc - 3), "ico", 3)) {
		req->type = IMAGE_REQUEST;
	} else {
		req->type = BAD_REQUEST;
	}

	return req;
}