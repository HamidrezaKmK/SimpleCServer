source_code="simpleWebServer.c"
exec_file="my_server"

path_to_exec_file="../bin/$(exec_file)"
path_to_content_files_unix := $(shell pwd)/../cont/
path_to_content_files_windows := $(shell pwd)/../WindowsCont/
NEW_PATH := $(shell pwd)/../bin:$

SHELL := /bin/bash

ifndef TYPE
	TYPE="multiProcess"
endif

ifndef PLATFORM
	PLATFORM="unix"
endif

all:
	@gcc -Wall -c logger.c;
	@gcc -Wall -c request.c;
	@gcc -Wall -c utils.c;
	@gcc -Wall -c config.c;
	@if [ $(TYPE) == 'multiThread' ] ; then \
		gcc -Wall -c -pthread multiThreadServer.c; \
		gcc -Wall -c -pthread -DMULTITHREAD serverUtils.c; \
		gcc -Wall -c -DMULTITHREAD -pthread simpleWebServer.c; \
		gcc -pthread -DMULTITHREAD -o $(path_to_exec_file) logger.o config.o request.o utils.o multiThreadServer.o serverUtils.o simpleWebServer.o; \
	elif [ $(TYPE) == 'multiProcess' ]; then \
		gcc -Wall -c multiProcessServer.c; \
		gcc -Wall -c -DMULTIPROC serverUtils.c; \
		gcc -Wall -c -DMULTIPROC simpleWebServer.c; \
		gcc -DMULTIPROC -o $(path_to_exec_file) logger.o config.o request.o utils.o multiProcessServer.o serverUtils.o simpleWebServer.o; \
	else \
		echo "nothing!"; \
	fi
	@echo "Server will work $(TYPE) on $(PLATFORM)";

install:
	@echo Installing ...
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
	@rm -f ../bin/*;
	@rm *.o
