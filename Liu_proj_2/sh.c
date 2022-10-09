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
#include <sys/stat.h>
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

void cd(char *owd, char *pwd, char *homedir, char **args, int argsct) {  
  char buffer[BUFFERSIZE];
  if (argsct == 2) {
    //chdir to directory previously in
    if (strcmp(args[1], "-") == 0) {
      chdir(pwd);
      strcpy(pwd, owd);
      getcwd(buffer, sizeof(buffer));
      strcpy(owd, buffer);
    }
    //chdir to directory
    else {
      struct stat statbuffer;
      int direxists = stat(args[1], &statbuffer);
      if ( direxists != -1 && S_ISDIR( statbuffer.st_mode ) ) {
        strcpy(pwd, owd);
        chdir(args[1]);
        getcwd(buffer, sizeof(buffer));
        strcpy(owd, buffer);
      } else {
        printf("%s is not a directory\n", args[1]);
      }
    }    
  }
  
  // chdir to home directory
  else if (argsct == 1) {
    strcpy(pwd, owd);
    chdir(homedir);
    getcwd(buffer, sizeof(buffer));
    strcpy(owd, buffer);
  }

  // too many arguments
  else {
    printf("Too many arguments entered\n");
  }
} /* cd() */

void pwd() {
  char buffer[BUFFERSIZE];
  getcwd(buffer, sizeof(buffer));
  printf("%s\n", buffer);
} /* pwd() */


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

void prompt(char *command, char *p) {
    char buffer[BUFFERSIZE];
    int len;
    if (command == NULL) {
        command = malloc(sizeof(char) * PROMPTMAX);
        printf("Input new prompt prefix: ");
        if (fgets(buffer, BUFFERSIZE, stdin) != NULL) {
            len = (int)strlen(buffer);
            buffer[len - 1] = '\0';
            strcpy(command, buffer);
        }
        strcpy(p, command);
        free(command);
    }
    else {
        strcpy(p, command);
    }
} /* prompt() */

void printenv(char **envp) {
  for(int i = 0; envp[i] != NULL; i++) {
    printf("%s\n", envp[i]);
  }
} /* printenv() */

void setenvi(char **args, int argsct) {
  if (argsct == 1) {
    printenv(args);
  } else if (argsct == 2) {
    setenv(args[1], "", 1);
  } else if (argsct == 3) {
    setenv(args[1], args[2], 1);
  } else {
    fprintf(stderr, "setenv can only take 2 arguments\n");
  }
} /* setenv() */