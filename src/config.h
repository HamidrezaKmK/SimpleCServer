#ifndef configFile_h
#define configFile_h

#include <stdio.h>

#define STRING_SIZE 256

struct Config {
    
    int PORTNUM,
    BUFSIZE,
    MAXPROCORTHREAD;
    
    char WebContentLocation[STRING_SIZE],
    StartingPageName[STRING_SIZE],
    LogFile[STRING_SIZE],
    ErrorHTML[STRING_SIZE],
    EXECUTABLE_LOCATION[STRING_SIZE];
};

struct Config* read_config_file(char *config_file_name);

void print_configurations(struct Config *conf);

#endif /* configFile_h */