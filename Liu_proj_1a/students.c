#include "students.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 128

Student *head = NULL;
Student *tail = NULL;

void add(char *lastname, char *firstname, long studentid, char *yearofstudent, int gradyear)
{
    Student *newNode = malloc(sizeof(Student));
    newNode->lastname = malloc(sizeof(char) * (strlen(lastname) + 1));
    strcpy(newNode->lastname, lastname);
    newNode->firstname = malloc(sizeof(char) * (strlen(firstname) + 1));
    strcpy(newNode->firstname, firstname);
    newNode->studentid = studentid;
    newNode->yearofstudent = malloc(sizeof(char) * (strlen(yearofstudent) + 1));
    strcpy(newNode->yearofstudent, yearofstudent);
    newNode->gradyear = gradyear;
    newNode->next = NULL;
    if (head == NULL)
    {
        newNode->prev = NULL;
        head = newNode;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
    }
    tail = newNode;
}

void delete (char *lastname)
{
    if (head == NULL && tail == NULL)
    {
        printf("Nothing to delete\n");
        return;
    }
    Student *curr = head;
    while (curr != NULL)
    {
        curr = curr->next;
        if (strcmp(curr->lastname, lastname) == 0)
        {
            if (curr == head)
            {
                curr->next->prev = NULL;
                head = curr->next;
            }
            else if (curr == tail)
            {
                curr->prev->next = NULL;
                tail = curr->prev;
            }
            else
            {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
            }
            free(curr->lastname);
            free(curr->firstname);
            free(curr->yearofstudent);
            free(curr);
            break;
        }
    }
}

void printinorder()
{
    Student *curr = head;
    while (curr != NULL)
    {
        printf("Last Name: %s, First Name: %s, Student ID: %ld, Year: %s, Graduation Year: %d\n", curr->lastname, curr->firstname, curr->studentid, curr->yearofstudent, curr->gradyear);
        curr = curr->next;
    }
}

void printreverse()
{
    Student *curr = tail;
    while (curr != NULL)
    {
        printf("Last Name: %s, First Name: %s, Student ID: %ld, Year: %s, Graduation Year: %d\n", curr->lastname, curr->firstname, curr->studentid, curr->yearofstudent, curr->gradyear);
        curr = curr->prev;
    }
}

void leave()
{
    Student *curr;
    while (head != NULL)
    {
        curr = head;
        head = head->next;
        free(curr->lastname);
        free(curr->firstname);
        free(curr->yearofstudent);
        free(curr);
    }
}

int main()
{
    int input = 0;
    char buffer[BUFFERSIZE];
    char lastname[BUFFERSIZE];
    char firstname[BUFFERSIZE];
    long studentid;
    char yearofstudent[BUFFERSIZE];
    int gradyear;

    while (input != 5)
    {
        printf("Input a number 1-5 to:\n(1) Add a student to the list,\n(2) Delete student(s) from the list\n(3) Print the list from beginning to end\n(4) Print the list from end to beginning\n(5) Exit the program\n");
        fgets(buffer, BUFFERSIZE, stdin);
        input = atoi(buffer);

        switch (input)
        {
        case 1:
            printf("(1) Adding student...\n");

            // read last name from stdin
            printf("Input student last name:\n");
            fgets(lastname, BUFFERSIZE, stdin);
            lastname[strlen(lastname) - 1] = '\0';

            // read first name from stdin
            printf("Input student first name:\n");
            fgets(firstname, BUFFERSIZE, stdin);
            firstname[strlen(firstname) - 1] = '\0';

            // read student ID from stdin
            printf("Input student ID:\n");
            fgets(buffer, BUFFERSIZE - 1, stdin);
            studentid = (long) atoi(buffer);

            // read student year from stdin
            printf("Input year of student:\n");
            fgets(yearofstudent, BUFFERSIZE, stdin);
            yearofstudent[strlen(yearofstudent) - 1] = '\0';

            // read graduation year from stdin
            printf("Input graduation year:\n");
            fgets(buffer, BUFFERSIZE, stdin);
            gradyear = atoi(buffer);

            add(lastname, firstname, studentid, yearofstudent, gradyear);

            break;
        case 2:
            printf("(2) Deleting student from list...\n");

            // read last name from stdin
            printf("Input student last name:\n");
            fgets(lastname, BUFFERSIZE, stdin);
            lastname[strlen(lastname) - 1] = '\0';

            delete(lastname);
            break;
        case 3:
            printf("Printing list from beginning to end...\n");
            printinorder();
            break;
        case 4:
            printf("Printing list from end to beginning...\n");
            printreverse();
            break;
        case 5:
            printf("Preparing to exit by freeing all dynamically allocated memory...\n");
            leave();
            break;
        default:
            printf("Invalid input\n");
            break;
        }

    }
}