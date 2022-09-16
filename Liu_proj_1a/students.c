#include <students.h>
#include <stdio.h>
#include <string.h>
#define MAX 128

Student* head = NULL;
Student* tail = NULL;

void add(char* lastname, char* firstname, long studentid, char* yearofstudent, int gradyear) {
    Student* newNode = malloc(sizeof(Student));
    newNode->lastname = lastname;
    newNode->firstname = firstname;
    newNode->studentid = studentid;
    newNode->yearofstudent = yearofstudent;
    newNode->gradyear = gradyear;
    newNode->next = NULL;
    if(head == NULL) {
        newNode->prev = NULL;
        head = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
    }
    tail = newNode;
}

void delete(char* lastname) {
    if(head == NULL && tail == NULL) {
        printf("Nothing to delete");
        return;
    }
    Student* curr = head;
    while(curr != NULL) {
        curr = curr->next;
        if(strcmp(curr->lastname, lastname) == 0) {
            if(curr == head) {
                curr->next->prev = NULL;
                head = curr->next;
            }
            else if(curr == tail) {
                curr->prev->next = NULL;
                tail = curr->prev;
            }
            else {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
            }
            free(curr);
            break;
        }
    }
}

void printinorder() {
    Student* curr = head;
    while(curr != NULL) {
        printf("Last Name: %s, First Name: %s, Student ID: %ld, Year: %s, Graduation Year: %d", curr->lastname, curr->firstname, curr->studentid, curr->yearofstudent, curr->gradyear);
        curr = curr->next;
    }
}

void printreverse() {
    Student* curr = tail;
    while(curr != NULL) {
        printf("Last Name: %s, First Name: %s, Student ID: %ld, Year: %s, Graduation Year: %d", curr->lastname, curr->firstname, curr->studentid, curr->yearofstudent, curr->gradyear);
        curr = curr->prev;
    }
}