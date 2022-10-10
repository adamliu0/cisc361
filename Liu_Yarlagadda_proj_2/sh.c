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
#include <glob.h>
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
  
  /* signal handlers */
  signal(SIGINT, sighandler);
  signal(SIGTSTP, sighandler);

  while ( go )
  {
    /* print your prompt */
    printf("%s [%s]>", prompt, pwd);
    /* get command line and process */
    argsct = getinput(args);
    /* check for each built in command and implement */

    /* exit */
    if (argsct == -1 || strcmp(args[0], "exit") == 0) {
      printf("Freeing dynamically allocated memory to prepare for exit...\n");
      free(pathlist->element);
      struct pathelement *temp;
      while (pathlist != NULL) {
        temp = pathlist;
        pathlist = pathlist->next;
        free(temp);
      }
      clearinput(args);
      free(prompt);
      free(args);
      free(pwd);
      free(owd);
      exit(1);
    }

    /* which */
    else if (strcmp(args[0], "which") == 0) {
      printf("which\n");
      if (args[1] == NULL) {
        printf("Error: which requires at least 1 argument\n");
      } else {
        if (which(args[1], pathlist)) {
          printf("Path found: %s\n", which(args[1], pathlist));
        } else {
          printf("%s: not found\n", args[1]);
        }
      }
    }

    /* where */
    else if (strcmp(args[0], "where") == 0) {
      printf("where\n");
      if (args[1] == NULL) {
        printf("Error: where requires at least 1 argument\n");
      } else {
        if (where(args[1], pathlist)) {
          printf("Path found: %s\n", where(args[1], pathlist));
        } else {
          printf("%s: not found\n", args[1]);
        }
      }
    }

    /* cd */
    else if (strcmp(args[0], "cd") == 0) {
      printf("cd\n");
      cd(owd, pwd, homedir, args, argsct);
    }

    /* pwd */
    else if (strcmp(args[0], "pwd") == 0) {
      printf("pwd\n");
      pwdfunc();
    }

    /* list */
    else if (strcmp(args[0], "list") == 0) {
      printf("list\n");
      list(owd);
    }

    /* pid */
    else if (strcmp(args[0], "pid") == 0) {
        printf("pid\n");
        ppid();
    }

    /* kill */
    else if (strcmp(args[0], "kill") == 0) {
      printf("kill\n");
      int killed = 1;
      if (argsct == 2) {
        killed = kill(atoi(args[1]), SIGTERM);
      }
      else if (argsct == 3) {
        if (args[1][0] == '-') {
          int signal = atoi(args[1] + 1);
          killed = kill(atoi(args[2]), signal);
        }
        else if (args[2][0] == '-') {
          int signal = atoi(args[2] + 1);
          killed = kill(atoi(args[1]), signal);
        }
      }
      if (killed) {
        perror("Error with kill");
      }
    }

    /* prompt */
    else if (strcmp(args[0], "prompt") == 0) {
      printf("prompt\n");
      promptfunc(command, p);
    }

    /* printenv*/
    else if (strcmp(args[0], "printenv") == 0) {
      printf("printenv\n");
      printenv(args);
    }

    /* setenv */
    else if (strcmp(args[0], "setenv") == 0) {
      printf("setenv\n");
      setenvi(args, argsct);
    }

    else
		{ // external command
			if ((pid = fork()) < 0)
			{
				printf("fork error");
			}
			else if (pid == 0)
			{ /* child */
				// an array of arguments for execve()
				char *execargs[MAXARGS];
				glob_t paths;
				int csource, j;
				char **p;
				struct pathelement *path = get_path();
				char *cmdPath;

				execargs[0] = malloc(argsct + 1);
				strcpy(execargs[0], args[0]); // copy command
				j = 1;
				for (i = 1; i < argsct; i++) // check arguments
				{
					if (strchr((char*)args[i], '*') != NULL)
					{ // wildcard
						csource = glob(arg+i, 0, NULL, &paths);
						if (csource == 0)
						{
							for (p = paths.gl_pathv; *p != NULL; ++p)
							{
								execargs[j] = malloc(strlen(*p) + 1);
								strcpy(execargs[j], *p);
								j++;
							}

							globfree(&paths);
						}
					}
					else // flags and directories
					{
						execargs[j] = malloc(strlen(args[i]));
						strcpy(execargs[j], args[i]);
						j++;
					}
				}
				execargs[j] = NULL;

                if (strncmp(args[0], "/", 1) == 0)
                {
                    if (access(args[0], X_OK) == 0)
                    {
						i = 0;
						for (i = 0; i < j; i++)
							printf("exec arg [%s]\n", execargs[i]);

						execve(execargs[0], execargs, NULL);
                        printf("couldn't execute: %s\n", args[0]);
                    }
					else
					{
						perror(args[0]);
					}
                }
                else
                {
					i = 0;
					for (i = 0; i < j; i++)
						printf("exec arg [%s]\n", execargs[i]);

					cmdPath = which(execargs[0], path);
                    execve(cmdPath, execargs, NULL);
					perror(args[0]);
				}
                exit(127);
			}

			/* parent */
			if ((pid = waitpid(pid, &status, 0)) < 0)
				printf("waitpid error");
			
            if (WIFEXITED(status))
                printf("child terminates with (%d)\n", WEXITSTATUS(status));
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

void pwdfunc() {
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

void ppid()
{
    printf("PID: %d\n", getpid());
} /* pid() */

void promptfunc(char *command, char *p) {
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

void clearinput(char **input) {
  for (int i = 0; i < MAXARGS; i++) {
    if (input[i] != NULL) {
      free( *(input+i) );
      input[i] = NULL;
    }
  }
}/* clearinput() */

int getinput(char **input) {
  // clear input
  clearinput(input);
  // get input
  char buffer[BUFFERSIZE];
  char *checkEOF;
  checkEOF = fgets(buffer, 127, stdin);
  if(checkEOF == NULL) {
    printf("\nEOF Detected\n");
    return -1;
  }
  buffer[strlen(buffer) - 1] = '\0';
  
  // chop strings and copy to args
  char *token;
  token = strtok(buffer, " ");
  int argsct;
  for(int i = 0; token != NULL && i < MAXARGS; i++) {
    input[i] = malloc( (1 + strlen(token) ) * sizeof(char));
    strcpy(input[i], token);
    token = strtok(NULL, " ");
    argsct = i;
  }
  return argsct;
} /* getinput() */

void globglob(int argsct, char **args, int index) {
  glob_t globbuffer;
  if (argsct == 1) {
    globfree(&globbuffer);
  } 
  else {
    int globstatus = glob(args[index], 0, NULL, &globbuffer);
    if (globstatus == GLOB_NOSPACE ) {
      printf("GLOB_NOSPACE\n");
    }
    if (globstatus == GLOB_ABORTED) {
      printf("GLOB_ABORTED\n");
    }
    if (globstatus == GLOB_NOMATCH) { 
      printf("GLOB_NOMATCH\n");
    }
    free(args[index]);
    for (int i = 0; globbuffer.gl_pathv[i] && (index+i) < MAXARGS; i++) {
      args[index+i] = malloc( (strlen(globbuffer.gl_pathv[i]) + 1) * sizeof(char));
      strcpy(args[index+i], globbuffer.gl_pathv[i]);
    }
    globfree(&globbuffer);
  }
}

/* signal handler*/
void sighandler(int sig) {
  if(sig == SIGINT) {
    signal(SIGINT, sighandler);
    printf("\n Cannot be terminated using Ctrl+C\n");
    fflush(stdout);
}
  else if(sig == SIGTSTP){
    signal(SIGTSTP, sighandler);
    printf("\n Cannot be terminated using Ctrl+Z\n");
    fflush(stdout);
  }
}
/* child signal handler*/
void childhandler(int sig) {
  if(sig = SIGINT) {
    exit(128 + SIGINT);
  }
  else if(sig == SIGTSTP) {
    exit(128 + SIGTSTP);
  }
}
