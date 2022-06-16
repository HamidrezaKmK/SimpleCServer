#ifndef LOGGER_H_   /* Include guard */
#define LOGGER_H_

#define LOG_SIZE 200000
#define MSG_SIZE 5000
#define TIME_SIZE 500
#define USER_INFO_SIZE 600

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

void add_to_log(struct Log *log);

char * get_all_logs();

#endif
