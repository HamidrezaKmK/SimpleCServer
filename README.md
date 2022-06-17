# Simple Web Server
### Project Explanation

In this project, we present a web server that serves the users' requests using multiple threads or processes. 

The project consists of the following parts:

######  Config file: 
We can set up the parameters website content, error page name, logger name, maximum workers, etc using the config file in `src/configurations/server.conf`.
- `PROTNUM`: This paramter is used to define the Port Number (default setting = 8080).
- `BUFSIZE`: The buffer size is for communication of sockets and between multiple processes (default setting = 25600).
- `EXECUTABLE_LOCATION`: This string defines the address of the file containing the executable (default setting = ../bin/).
- `StartingPageName`: This string defines the starting web location (default setting = main.html).
- `ErrorHTML`: This string defines the error web location(default setting = errorPage.html). This page pops up if the input website is not defined.
- `LogFile`: This string defines the name of the .text file that stores the logs (default setting = logfile.txt).
- `WebContentLocation`: This string defines the address of the web content location where the html files are located (default setting = ../cont-enhanced/).
- `MAXPROCORTHREAD`: Multiprocessing configuration (default setting = 3).
###### .c codes
The codes are in the src folder. All of them contain necessary function that are used in the main `simpleWebServer.c` file.
###### Makefile
We can compile and bind th .c files using Makefile.

### How to Run the Project?
Please follow this procedure:
1. Go to the `src` directory.
2. Run `make clean` in case you want to delete the previous log files.
3. Run `make all TYPE=(TYPE) PLATFORM=__(PLATFORM)__`, where TYPE can be multiProcess or multiThread, and PLATFORM can be either MAC or LINUX.
4. Run `make install` to place the executable file in `/usr/local/bin/`.
5. From now on you can easily run `simple-c-server` to set up the web server.
6. Open Firefox browser and insert `localhost:8080`.

### Webpages

There are 3 webpages in this project:
1. The main page `main.html`. This page contains the _Project Explanation_ section.
2. The second page `secondpage.html`. This page contains the _How to Run the Project?_ section.
3. The error page `error.html`. This page comes if you enter a wrong address.