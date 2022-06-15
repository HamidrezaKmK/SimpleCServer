source_code="simpleWebServer.c"
exec_file="my_server"

path_to_exec_file="../bin/$(exec_file)"
path_to_content_files_unix := $(shell pwd)/../cont/
path_to_content_files_windows := $(shell pwd)/../WindowsCont/
NEW_PATH := $(shell pwd)/../bin:$

SHELL := /bin/bash

ifndef TYPE
	TYPE="multiThread"
endif

ifndef PLATFORM
	PLATFORM="unix"
endif

all:
	@gcc -Wall -c logger.c
	@gcc -Wall -c request.c
	@gcc -Wall -c utils.c
	@gcc -Wall -c simpleWebServer.c
	@if [ $(TYPE) == 'multiThread' ] ; then \
		gcc -Wall -c multiThreadServer.c; \
		gcc -o $(path_to_exec_file) -DMULTITHREAD -pthread logger.o request.o utils.o multiThreadServer.o simpleWebServer.o; \
	else \
		echo "nothing!"; \
	fi
	@echo "Server will work $(TYPE) on $(PLATFORM)";

install:
#@echo Installing ...;
	@mkdir -p ../bin;
#	@echo Add to PATH;
#	@echo export PATH="$(NEW_PATH)"PATH >> ~/.bashrc;
#	@source ~/.bashrc;
#	@echo Installed;
	@echo Add conf to /etc/simpleWebServer;
	@mkdir -p /etc/simpleWebServer/
	@rm -f /etc/simpleWebServer/*
	@cp -r configurations/server.conf configurations/WindowsServer.conf /etc/simpleWebServer
	@echo Add content address to conf
	@echo WebContentLocation = $(path_to_content_files_unix) >> /etc/simpleWebServer/server.conf
	@echo WebContentLocation = $(path_to_content_files_windows) >> /etc/simpleWebServer/WindowsServer.conf
	@echo Done
clean:
	@rm -f $(path_to_exec_file);
	@rm *.o
