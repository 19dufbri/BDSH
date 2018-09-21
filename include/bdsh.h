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
#define fileExecute(x)	access((x), X_OK) == 0
#define fileExist(x)	access((x), F_OK) == 0
#define BUF_SIZE	100

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

// Colors for output
#define BOLDC		"\x1b[1m"
#define GREENC		BOLDC "\x1b[38;5;46m" 
#define BLUEC		BOLDC "\x1b[38;5;33m"
#define REDC		BOLDC "\x1b[38;5;196m"
#define RESETC		"\x1b[0m"

// Main function declarations
char *getLineInput(void);
char *findProgPath(char *prgm);
char **getAllArgs(char *line);
char *getFullPath(char *path, char *prog);
int createChildProcess(char* prgm, char *argv[]);
void allocerror(void);

// Get the environment 
extern char **environ;

// Return Code for BDSH
extern int code;

#endif