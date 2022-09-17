#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct student {
  char* lastname;
  char* firstname;
  long studentid;
  char* yearofstudent;
  int gradyear;
  struct student* next;
  struct student* prev;
} Student;

void add(char* lastname, char* firstname, long studentid, char* yearofstudent, int gradyear);
void delete(char* lastname);
void printinorder();
void printreverse();
void leave();