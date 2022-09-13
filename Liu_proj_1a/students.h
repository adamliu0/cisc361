typedef struct student {
  char* lastname;
  char* firstname;
  long studentid;
  char* yearofstudent;
  int gradyear;
  Student* next;
  Student* prev;
} Student;

typedef struct slist {
  Student* head;
  Student* tail;
} Slist;

void add();
void remove();
void printinorder();
void printreverse();
void exit();

