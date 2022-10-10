
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void cd(char *owd, char *pwd, char *homedir, char **args, int argsct);
void pwdfunc();
void list ( char *dir );
void ppid();
void promptfunc(char *command, char *p);
void printenv(char **envp);
void setenvi(char **args, int argsct);
void clearinput(char **input);
int getinput(char **input);

void sig_handler(int signal); 

#define PROMPTMAX 32
#define MAXARGS 10
