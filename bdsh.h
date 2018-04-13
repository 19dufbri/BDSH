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
#define BIHELPC		PATHC
#define RESETC          "\x1b[0m"

// Main function declarations
char *getLineInput(void);
char *findProgPath(char *prgm);
char **getAllArgs(char *line);
char *getFullPath(char *path, char *prog);
int createChildProcess(char* prgm, char *argv[]);
void allocerror(void);

// Get the environment 
extern char **environ;

// Builtin Functions
#define NUM_BUILTINS    (sizeof(builtins) / sizeof(struct builtin))

bool checkBuiltins(char *argv[]);
void builtinexit(int argc, char *argv[]);
void builtincd(int argc, char *argv[]);
void builtinhelp(int argc, char *argv[]);

struct builtin {
	char *name;
	char *help;
	void (*function)(int, char **);
};

#endif