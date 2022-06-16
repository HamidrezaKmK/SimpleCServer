
exec_file="simple-c-server"

exec_dir := "../bin/$(exec_file)"
content_dir := $(shell pwd)/../cont/
NEW_PATH := $(shell pwd)/../bin:$

SHELL := /bin/bash

ifndef TYPE
	TYPE="multiProcess"
endif

ifndef PLATFORM
	PLATFORM="__LINUX__"
endif

all:
	@mkdir -p obj
	@gcc -Wall -c logger.c -o obj/logger.o;
	@gcc -Wall -c request.c -o obj/request.o;
	@if [ $(PLATFORM) == "__LINUX__" ] ; then \
		gcc -Wall -c -D__LINUX__ utils.c -o obj/utils.o; \
	elif [ $(PLATFORM) == "__MAC__" ] ; then \
		gcc -Wall -c -D__MAC__ utils.c -o obj/utils.o; \
	else \
		echo "PLATFORM $PLATFORM incorrect"; \
	fi
	@gcc -Wall -c config.c -o obj/config.o;
	@if [ $(TYPE) == 'multiThread' ] ; then \
		gcc -Wall -c -pthread multiThreadServer.c -o obj/multiThreadServer.o; \
		gcc -Wall -c -pthread -DMULTITHREAD serverUtils.c -o obj/serverUtils.o; \
		gcc -Wall -c -DMULTITHREAD -pthread simpleWebServer.c -o obj/simpleWebServer.o; \
		gcc -pthread -DMULTITHREAD -o $(exec_dir) obj/logger.o obj/config.o obj/request.o obj/utils.o obj/multiThreadServer.o obj/serverUtils.o obj/simpleWebServer.o; \
	elif [ $(TYPE) == 'multiProcess' ]; then \
		gcc -Wall -c multiProcessServer.c -o obj/multiProcessServer.o; \
		gcc -Wall -c -DMULTIPROC serverUtils.c -o obj/serverUtils.o; \
		gcc -Wall -c -DMULTIPROC simpleWebServer.c -o obj/simpleWebServer.o; \
		gcc -DMULTIPROC -o $(exec_dir) obj/logger.o obj/config.o obj/request.o obj/utils.o obj/multiProcessServer.o obj/serverUtils.o obj/simpleWebServer.o; \
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
	@mkdir -p /etc/simpleWebServer/
	@rm -f /etc/simpleWebServer/*
	@cp -r configurations/server.conf configurations/WindowsServer.conf /etc/simpleWebServer
	@echo Add content address to conf
	@echo WebContentLocation = $(path_to_content_files_unix) >> /etc/simpleWebServer/server.conf
	@echo WebContentLocation = $(path_to_content_files_windows) >> /etc/simpleWebServer/WindowsServer.conf
	@echo Done
clean:
	@rm -f ../bin/*;
	@rm -rf obj