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