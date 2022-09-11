typedef struct student {
  char* lastname;
  char* firstname;
  long studentid;
  char* yearofstudent;
  int gradyear;
  Student* next;
  Student* prev;
} Student;
