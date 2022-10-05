#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#define BUFFERSIZE 1028

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/
     
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  while ( go )
  {
    /* print your prompt */
    
    /* get command line and process */

    /* check for each built in command and implement */

     /*  else  program to exec */
    {
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    }
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   char *buffer = malloc(BUFFERSIZE);
   while(pathlist) {
    sprintf(buffer, "%s:%s", pathlist->element, command);
    if(access(buffer, X_OK) == 0) {
      return buffer;
    }
    pathlist = pathlist->next;
   }
   free(buffer);
   return NULL;
} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  char *buffer = malloc(BUFFERSIZE);
   while(pathlist) {
    sprintf(buffer, "%s:%s", pathlist->element, command);
    if(access(buffer, F_OK) == 0) {
      return buffer;
    }
    pathlist = pathlist->next;
   }
   free(buffer);
   return NULL;
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  DIR *d;
  struct dirent *dn;
  d = opendir(dir);
  if(d) {
    while(dn = readdir(d)) {
      printf("%s\n", dn->d_name);
    }
  }
  else {
    perror(dir);
  }
} /* list() */

void pwd() {
  char buffer[BUFFERSIZE];
  getcwd(buffer, sizeof(buffer));
  printf("%s\n", buffer);
}

void printenv(char **envp) {
  char **env = envp;
  if(sizeof(env) == 0) {
    while(*env ) {
      printf("%s\n", env);
      env++;
    }
  }
  else if(sizeof(env) == 1) {
    if(env) {
      printf("%s\n", env);
    }
    else {
      printf("Environment variable does not exist");
    }
  }
}