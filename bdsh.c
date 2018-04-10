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

// Our builtin functions
struct builtin builtins[] = {
  {"exit", &builtinexit},
  {"cd", &builtincd},
  {"help", &builtinhelp}
};

// Main function
int main(int argc, char *argv[]) {
    
    char *line;
    char *path;
    char **args;
    
    // Main loop of shell
    while (true) {
        // Prompt
        printf(USERC "%s" RESETC ":" PATHC "%s" RESETC "> ", getpwuid(getuid())->pw_name, getcwd(NULL, 0));
        fflush(stdout);
        // Get our arguments
        line = getLineInput();
        if (*line == 0x0) {
            free(line);
            continue;
        }
        args = getAllArgs(line);
        // Try to execute
        if (checkBuiltins(args)) {
            free(args);
            free(line);
            continue;
        }
        path = findProgPath(args[0]);
        if (path != NULL) {
            createChildProcess(path, args);
        }
        // Memory Management
        free(args);
        free(line);
    }
    exit(0);
}

// Get a single line of stdin
char *getLineInput(void) {
    
    char *line;
    int cur;
    int used;
    
    used = 0;
    line = calloc(BUF_SIZE, sizeof(char));
    
    if (line == NULL) allocerror();
    
    while ((cur = getchar()) != EOF) {
        if (used % BUF_SIZE == 0) {
            line = realloc(line, (BUF_SIZE + used) * sizeof(char));
            if (line == NULL) allocerror();
        }
        if (cur == '\n') break;
        line[used] = cur;
        used++;
    }
    
    return line;
}

// Break string by spaces
char **getAllArgs(char *line) {
    
    char **argv;
    char *start;
    char cur;
    int size;
    int i;
    bool space;
    
    argv = calloc(1, sizeof(char *));
    
    if (argv == NULL) allocerror();
    
    i = 0;
    start = line;
    size = 0;
    cur = 'A';
    
    while (cur != 0x0) {
        cur = line[i];
        if (cur == ' ' || cur == 0x0) {
            if (!space) {
                argv = realloc(argv, (size + 2) * sizeof(char *));
                
                if (argv == NULL) allocerror();
                
                argv[size] = start;
                argv[size + 1] = NULL;
                size++;
            }
            line[i] = 0x0;
            start = line + i + 1;
            space = true;
        } else {
            space = false;
        }
        i++;
    }
    
    return argv;
}

// Find the path to a specific program
char *findProgPath(char *prgm) {
    
    char *path;
    char *start;
    char *result;
    char cur;
    int i;
    
    start = getenv("PATH");
    path = calloc(strlen(start) + 1, sizeof(char));
    
    if (path == NULL) allocerror();
    
    strcat(path, start);
    
    i = 0;
    start = path;
    
    // Assume in PATH
    while (true) {
        cur = path[i];
        if (cur == ':' || cur == 0x0) {
            path[i] = 0x0;
            result = getFullPath(start, prgm);
            if (fileExist(result)) {
                free(path);
                if (fileExecute(result)) return result;
                free(result);
                fprintf(stderr, WARNC "%s: You don't have executable permissions\n" RESETC, prgm);
                return NULL;
            }
            if (cur == 0x0) break;
            start = path + i + 1;
        }
        i++;
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
            fprintf(stderr, WARNC "%s: You don't have executable permissions\n" RESETC, prgm);
        }
    }
    
    fprintf(stderr, WARNC "%s: Program couldn't be found\n" RESETC, prgm);
    return NULL;
}

// Get the full path of a program
char *getFullPath(char *path, char *prog) {
    
    char *result;
    
    result = calloc(strlen(path) + strlen(prog) + 2, sizeof(char));
    
    if (result == NULL) allocerror();
    
    strcat(result, path);
    strcat(result, "/");
    strcat(result, prog);
    
    return result;
}

// Get the environment 
extern char **environ;

// Attempt to create a child process
int createChildProcess(char* prgm, char *argv[]) {
    
    int pid;
    int status;
    
    if ((pid = fork()) != 0) {
        // Parent process
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) {
            fprintf(stderr, WARNC "%s: Exited with code: %i\n" RESETC, argv[0], WEXITSTATUS(status));
        }
        return WEXITSTATUS(status);
    } else {
        // Child process
        execve(prgm, argv, environ);
        fprintf(stderr, WARNC "%s: Couldn't be started\n" RESETC, argv[0]);
        exit(0);
    }
    return -1;
}

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
    if (argc == 2) {
        exit(atoi(argv[1]));
    } else if (argc == 1) {
        exit(0);
    } else {
        fprintf(stderr, WARNC "exit: Too many arguments\n" RESETC);
    }
}

// Changes the current directory
void builtincd(int argc, char *argv[]) {
    
    char *home = getenv("HOME");
    char *path;
    
    if (argc >= 1 && argc <= 2) {
        if (argc == 1) {
            if (home == NULL) {
                fprintf(stderr, WARNC "cd: Folder doesn't exist" RESETC);
                return;
            }
        path = home;
        } else {
            path = argv[1];
        }
        errno = 0;
        if (chdir(path) == -1) {
            if (errno == EACCES) {
                fprintf(stderr, WARNC "cd: You don't have permission\n" RESETC);
            } else if (errno == ENOENT){
                fprintf(stderr, WARNC "cd: \"%s\" doesn't exist\n" RESETC, path);
            } else if (errno == ENOTDIR) {
                fprintf(stderr, WARNC "cd: \"%s\" is not a directory\n" RESETC, path);
            } else if (errno != 0) {
                fprintf(stderr, WARNC "cd: Unknown error occured\n" RESETC);
            }
        }
    } else {
        fprintf(stderr, WARNC "cd: Too many arguments\n" RESETC);
    }
}

// Provide help for shell builtins
void builtinhelp(int argc, char *argv[]) {
    printf(BINAMEC "===== Help =====\n");
    printf(BINAMEC "cd: "   BIDESCC "Change the current working directory\n");
    printf(BINAMEC "exit: " BIDESCC "Exits the shell environment\n");
    printf(BINAMEC "help: " BIDESCC "Gives hap on builtin shell commands\n");
    printf(BINAMEC "================\n" RESETC);
}

// Endpoint for memory erors
void allocerror(void) {
    fprintf(stderr, WARNC "bdsh: Couldn't allocate memory\n" RESETC);
    exit(1);
}