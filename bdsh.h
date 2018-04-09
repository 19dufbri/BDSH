#ifndef BDSH
#define BDSH

#define fileExecute(x)  access((x), X_OK) == 0
#define fileExist(x)    access((x), F_OK) == 0
#define BUF_SIZE        100
#define ZEROCHAR        '0'

#define BOLDC           "\x1b[1m"
#define USERC           BOLDC "\x1b[38;5;46m" 
#define PATHC           BOLDC "\x1b[38;5;33m"
#define WARNC           BOLDC "\x1b[38;5;196m"
#define BINAMEC         USERC
#define BIDESCC         PATHC
#define RESETC          "\x1b[0m"

char *getLineInput();
char *findProgPath(char *prgm);
char **getAllArgs(char *line);
bool prgmInDir(char *dirName, char *fileSearch);
char *getFullPath(char *path, char *prog);
int createChildProcess(char* prgm, char *argv[]);

bool checkBuiltins(char *argv[]);
void builtinexit(int argc, char *argv[]);
void builtincd(int argc, char *argv[]);
void builtinhelp(int argc, char *argv[]);

#endif