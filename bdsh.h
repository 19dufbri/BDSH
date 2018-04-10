#ifndef BDSH
#define BDSH

// Normal Macros
#define fileExecute(x)  access((x), X_OK) == 0
#define fileExist(x)    access((x), F_OK) == 0
#define BUF_SIZE        100
#define ZEROCHAR        '0'

// Colors for output
#define BOLDC           "\x1b[1m"
#define USERC           BOLDC "\x1b[38;5;46m" 
#define PATHC           BOLDC "\x1b[38;5;33m"
#define WARNC           BOLDC "\x1b[38;5;196m"
#define BINAMEC         USERC
#define BIDESCC         PATHC
#define RESETC          "\x1b[0m"

// Main function declarations
char *getLineInput(void);
char *findProgPath(char *prgm);
char **getAllArgs(char *line);
char *getFullPath(char *path, char *prog);
int createChildProcess(char* prgm, char *argv[]);
void allocerror(void);

// Builtin Functions
#define NUM_BUILTINS    (sizeof(builtins) / sizeof(struct builtin))

bool checkBuiltins(char *argv[]);
void builtinexit(int argc, char *argv[]);
void builtincd(int argc, char *argv[]);
void builtinhelp(int argc, char *argv[]);

struct builtin {
    char *name;
    void (*function)(int, char **);
};

#endif