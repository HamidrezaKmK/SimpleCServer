
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"

char all_logs[LOG_SIZE];

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

void add_to_log(struct Log *log) {
	strcat(all_logs, "TIME: ");
	strcat(all_logs, log->time);
	strcat(all_logs, "USER: ");
	strcat(all_logs, log->user_info);
	strcat(all_logs, "MESSAGE_HISTORY:\n");
	strcat(all_logs, log->msg);
}

char * get_all_logs() {
	return all_logs;
}