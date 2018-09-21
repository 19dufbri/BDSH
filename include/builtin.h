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

#ifndef BUILTIN
#define BUILTIN

// Builtin Functions
#define NUM_BUILTINS	(sizeof(builtins) / sizeof(struct builtin))

struct builtin {
	char *name;
	char *help;
	void (*function)(int, char **);
};

extern struct builtin builtins[];

bool checkBuiltins(char *argv[]);
void builtinexit(int argc, char *argv[]);
void builtincd(int argc, char *argv[]);
void builtinhelp(int argc, char *argv[]);

#endif