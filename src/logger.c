
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"

char all_logs[LOG_SIZE];
char intermediate_log[MSG_SIZE + TIME_SIZE + USER_INFO_SIZE + 100];

void settime(struct Log * log) {
	
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strcat(log->time, asctime(timeinfo));
}

void clearlog(struct Log * log) {
	if (log->initialized == 0) {
		log->msg = (char *) malloc(MSG_SIZE);
		log->time = (char *) malloc(TIME_SIZE);
		log->user_info = (char *) malloc(USER_INFO_SIZE);
		log->initialized = 1;
	}
	memset(log->msg, 0, MSG_SIZE);
	memset(log->time, 0, TIME_SIZE);
	memset(log->user_info, 0, USER_INFO_SIZE);
	log->msg_cursor = 0;
}

void add_msg(struct Log *log, char *msg) {
	char *pre = "\t- ";
	snprintf(log->msg + log->msg_cursor, MSG_SIZE, pre);
	snprintf(log->msg + log->msg_cursor + strlen(pre), MSG_SIZE, msg);
	snprintf(log->msg + log->msg_cursor + strlen(pre) + strlen(msg), MSG_SIZE, "\n");
	log->msg_cursor = strlen(log->msg);
}

void set_user_info(struct Log *log, char *user_info) {
	snprintf(log->user_info, USER_INFO_SIZE, user_info);
}

char * streamify(struct Log *log) {
	memset(intermediate_log, 0, sizeof(intermediate_log));
	strcat(intermediate_log, "TIME: ");
	strcat(intermediate_log, log->time);
	strcat(intermediate_log, "USER: ");
	strcat(intermediate_log, log->user_info);
	strcat(intermediate_log, "MESSAGE_HISTORY:\n");
	strcat(intermediate_log, log->msg);
	return intermediate_log;
}

void add_stream_to_log(char *stream) {
	strcat(all_logs, stream);
}

void add_to_log(struct Log *log) {
	char *stream = streamify(log);
	add_stream_to_log(stream);
}

char * get_all_logs() {
	return all_logs;
}