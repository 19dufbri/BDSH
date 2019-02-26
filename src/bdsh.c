/*  Copyright 2018 Brian Duffy
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include "bdsh.h"
#include "builtin.h"

int code = 0;

// Main function
int main(int argc, char *argv[]) {
	
	char *line, *path;
	char **args;
	
	// Main loop of shell
	while (true) {
		// Prompt
		char *cwd = getcwd(NULL, 0);
		printf(GREENC "%s" RESETC ":" BLUEC "%s" RESETC " (%d)> ", getpwuid(getuid())->pw_name, cwd, code);
		free(cwd);
		fflush(stdout);
		// Get our arguments
		line = getLineInput();
		if (*line == 0x0) {
			free(line);
			continue;
		}
		code = 0;
		args = getAllArgs(line);
		// Try to execute
		if (checkBuiltins(args)) {
			free(args);
			free(line);
			continue;
		}
		path = findProgPath(args[0]);
		if (path != NULL)
			code = createChildProcess(path, args);
		
		// Memory Management
		free(args);
		free(line);
	}
	exit(EXIT_SUCCESS);
}

// Get a single line of stdin
char *getLineInput(void) {
	
	char *line = calloc(BUF_SIZE, sizeof(char));
	int cur, used;
	
	if (line == NULL) allocerror();
	
	for (used = 0; (cur = getchar()) != EOF; used++) {
		if (used % BUF_SIZE == 0) {
			line = realloc(line, (BUF_SIZE + used) * sizeof(char));
			if (line == NULL) allocerror();
		}
		if (cur == '\n') break;
		line[used] = cur;
	}
	
	return line;
}

// Break string by spaces
char **getAllArgs(char *line) {
	
	char **argv = calloc(1, sizeof(char *));
	char *start = line;
	char cur;
	int size = 0;
	int i;
	bool space = false;
	
	if (argv == NULL) allocerror();
	
	for (i = 0; (cur = line[i]) || true; i++) {
		if (cur == ' ' || cur == 0x0) {
			if (!space) {
				argv = realloc(argv, (size + 2) * sizeof(char *));
				if (argv == NULL) allocerror();
				
				argv[size] = start;
				argv[++size] = NULL;
			}
			if (cur == 0x0) break;
			line[i] = 0x0;
			start = line + i + 1;
			space = true;
		} else {
			space = false;
		}
	}
	
	return argv;
}

// Find the path to a specific program
char *findProgPath(char *prgm) {
	
	char *start = getenv("PATH");
	char *path = calloc(strlen(start) + 1, sizeof(char));
	char *result;
	char cur;
	int i;
	
	if (path == NULL) allocerror();
	
	strcat(path, start);
	start = path;
	
	// Assume in PATH
	for (i = 0; (cur = path[i]) || true; i++) {
		if (cur == ':' || cur == 0x0) {
			path[i] = 0x0;
			result = getFullPath(start, prgm);
			if (fileExist(result)) {
				free(path);
				if (fileExecute(result)) return result;
				free(result);
				fprintf(stderr, REDC "%s: You don't have executable permissions\n" RESETC, prgm);
				code = 127;
				return NULL;
			}
			if (cur == 0x0) break;
			start = path + i + 1;
		}
	}
	
	free(result);
	free(path);
	
	// Not in PATH, relative path?
	if (fileExist(prgm)) {
		if (fileExecute(prgm)) {
			path = calloc(strlen(prgm) + 1, sizeof(char));
			
			if (path == NULL) allocerror();
			
			strcat(path, prgm);
			return path;
		} else {
			fprintf(stderr, REDC "%s: You don't have executable permissions\n" RESETC, prgm);
			code = 127;
			return NULL;
		}
	}
	
	fprintf(stderr, REDC "%s: Program couldn't be found\n" RESETC, prgm);
	code = 126;
	return NULL;
}

// Get the full path of a program
char *getFullPath(char *path, char *prog) {
	
	char *result = calloc(strlen(path) + strlen(prog) + 2, sizeof(char));
	
	if (result == NULL) allocerror();
	
	strcat(result, path);
	strcat(result, "/");
	strcat(result, prog);
	
	return result;
}

// Attempt to create a child process
int createChildProcess(char* prgm, char *argv[]) {
	
	int pid, status;
	
	if ((pid = fork()) != 0) {
		// Parent process
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	} else {
		// Child process
		execve(prgm, argv, environ);
		fprintf(stderr, REDC "%s: Couldn't be started\n" RESETC, argv[0]);
		exit(0); // Don't display another error message
	}
	return EXIT_FAILURE;
}

// Endpoint for memory erors
void allocerror(void) {
	fprintf(stderr, REDC "bdsh: Couldn't allocate memory\n" RESETC);
	exit(EXIT_FAILURE);
}
