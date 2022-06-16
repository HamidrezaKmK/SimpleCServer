#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 256

struct Config* read_config_file(char *config_file_name) {
    
    struct Config *config = (struct Config *) malloc(sizeof(struct Config));
    
    FILE *config_file_text = fopen(config_file_name, "r");
    
    if (config_file_text == NULL) {
        printf("Config file not defined or wrong path given: %s", config_file_name);
        exit(1);
    }
    
    char line[LINE_SIZE];
    
    while (fgets(line, LINE_SIZE, config_file_text)) {
        
        if (line[0] == '#' || line[0] == '\n') {
            continue;
        }
        
        char *token_1 = strtok(line, " = ");
        char *token_2 = strtok(NULL, " =\n");
        
        if (strcmp(token_1, "PORTNUM") == 0) {
            config->PORTNUM = atoi(token_2);
        } else if (strcmp(token_1, "BUFSIZE") == 0) {
            config->BUFSIZE = atoi(token_2);
        } else if (strcmp(token_1, "MAXPROCORTHREAD") == 0) {
            config->MAXPROCORTHREAD = atoi(token_2);
        } else if (strcmp(token_1, "WebContentLocation") == 0) {
            strcpy(config->WebContentLocation, token_2);
        } else if (strcmp(token_1, "StartingPageName") == 0) {
            strcpy(config->StartingPageName, token_2);
        } else if (strcmp(token_1, "LogFile") == 0) {
            strcpy(config->LogFile, token_2);
        } else if (strcmp(token_1, "ErrorHTML") == 0) {
            strcpy(config->ErrorHTML, token_2);
        } else if (strcmp(token_1, "EXECUTABLE_LOCATION") == 0) {
            strcpy(config->EXECUTABLE_LOCATION, token_2);
        }
    }
    
    fclose(config_file_text);
    return config;
}

void print_configurations(struct Config *conf) {
    printf("---- CONFIG ----\n");
    printf("\t-port number: %d\n", conf->PORTNUM);
    printf("\t-buffer size: %d\n", conf->BUFSIZE);
    printf("\t-starting page name: %s\n", conf->StartingPageName);
    printf("\t-web content: %s\n", conf->WebContentLocation);
    printf("---- ------ ----\n");
}