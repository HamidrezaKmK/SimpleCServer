#ifndef LOGGER_H_   /* Include guard */
#define LOGGER_H_

#define LOGSIZE 5000

struct Log {
	char *msg;
	size_t msg_cursor;
	char *time;
	char *user_info;
	int initialized;
};

void settime(struct Log * log);

void add_msg(struct Log * log, char * msg);

void clearlog(struct Log * log);

void set_user_info(struct Log *log , char *user_info);

#endif
