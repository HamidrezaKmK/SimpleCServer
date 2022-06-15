
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"

void settime(struct Log * log) {
	
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	log->time = asctime(timeinfo);
}

void clearlog(struct Log * log) {
	if (log->initialized == 0) {
		log->msg = (char *) malloc(LOGSIZE);
		log->time = (char *) malloc(LOGSIZE);
		log->user_info = (char *) malloc(LOGSIZE);
		log->initialized = 1;
	}
	memset(log->msg, 0, LOGSIZE);
	memset(log->time, 0, LOGSIZE);
	memset(log->user_info, 0, LOGSIZE);
}

void add_msg(struct Log *log, char *msg) {
	int t = strlen(msg);
	msg[t] = '\n';
	msg[t + 1] = '\0';
	snprintf(log->msg + log->msg_cursor, LOGSIZE, msg);
	log->msg_cursor = strlen(log->msg);
}

void set_user_info(struct Log *log, char *user_info) {
	snprintf(log->user_info, LOGSIZE, user_info);
}

