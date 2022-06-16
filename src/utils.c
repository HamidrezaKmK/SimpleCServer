#include "utils.h"

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}


// This function returns the absolute path of the location.
// In order to do that, it concats the content_location (i.e. absolute path to content folder) and the location.

char* path_join(char* content_location, char *location){
	char *rel_location=malloc(strlen(location)+1+strlen(content_location));
	strcpy(rel_location, content_location);
	strcat(rel_location, location);
	return rel_location;
}


// This function reads the content of a webpage to send it the client.

char *read_webpage(char *webpage_location)
{
	FILE *f = fopen(webpage_location, "r");
	if (f == NULL) {
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *webpage = malloc(fsize + 1);
	fread(webpage, fsize, 1, f);
	fclose(f);
	webpage[fsize] = 0;
	return webpage;
}