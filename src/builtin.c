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

// Our builtin functions
struct builtin builtins[] = {
  {"exit", GREENC "exit [code]" RESETC ": Exit the shell with [code].", &builtinexit},
  {"cd", GREENC "cd [dir]" RESETC ": Change current directory to [dir].", &builtincd},
  {"help", GREENC "help [command]" RESETC ": Get help with builtin commands.", &builtinhelp}
};

// Check if a command is a builtin
bool checkBuiltins(char *argv[]) {
	
	struct builtin search;
	int i;
	
	for (i = 0; i < NUM_BUILTINS; i++) {
		search = builtins[i];
		if (strcmp(search.name, argv[0]) == 0) {
			for (i = 0; argv[i] != NULL; i++)
				;
			
			search.function(i, argv);
			
			return true;
		}
	}
	
	return false;
}

// Exits the shell
void builtinexit(int argc, char *argv[]) {
	if (argc == 2)
		exit(atoi(argv[1]));
	else if (argc == 1)
		exit(EXIT_SUCCESS);
	
	fprintf(stderr, REDC "exit: Too many arguments\n" RESETC);
	code = 127;
}

// Changes the current directory
void builtincd(int argc, char *argv[]) {
	
	char *home = getenv("HOME");
	char *path;
	
	if (argc >= 1 && argc <= 2) {
		if (argc == 1) {
			if (home == NULL) {
				fprintf(stderr, REDC "cd: HOME doesn't exist" RESETC);
				code = 126;
				return;
			}
			path = home;
		} else {
			path = argv[1];
		}
		errno = 0;
		if (chdir(path) == -1) {
			if (errno == EACCES)
				fprintf(stderr, REDC "cd: You don't have permission\n" RESETC);
			else if (errno == ENOENT)
				fprintf(stderr, REDC "cd: \"%s\" doesn't exist\n" RESETC, path);
			else if (errno == ENOTDIR)
				fprintf(stderr, REDC "cd: \"%s\" is not a directory\n" RESETC, path);
			else if (errno != 0)
				fprintf(stderr, REDC "cd: Unknown error occured\n" RESETC);
			code = errno;
		}
	} else {
		fprintf(stderr, REDC "cd: Too many arguments\n" RESETC);
		code = 127;
	}
}

// Provide help for shell builtins
void builtinhelp(int argc, char *argv[]) {
	
	struct builtin search;
	int i;
	
	if (argc > 1) {
		for (i = 0; i < NUM_BUILTINS; i++) {
			search = builtins[i];
			if (strcmp(search.name, argv[1]) == 0) {
				printf("%s\n", search.help);
				return;
			}
		}
		fprintf(stderr, REDC "help: Command %s not found\n" RESETC, argv[1]);
		code = 127;
	}
	
	printf(BLUEC "===== BDSH Help =====\n" RESETC);
	for (i = 0; i < NUM_BUILTINS; i++)
		printf("%s\n", builtins[i].help);
	printf(BLUEC "=====================\n" RESETC);
}