typedef struct student {
  char* lastname;
  char* firstname;
  long studentid;
  char* yearofstudent;
  int gradyear;
  Student* next;
  Student* prev;
} Student;

void add(char* lastname, char* firstname, long studentid, char* yearofstudent, int gradyear);
void delete(char* lastname);
void printinorder();
void printreverse();
void exit();