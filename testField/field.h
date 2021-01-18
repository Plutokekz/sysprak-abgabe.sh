#ifndef BASCHNI_FIELD_H
#define BASCHNI_FIELD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
    char piece;
    struct node *next;
} node;

typedef struct queue {
    node *head;
    node *tail;
} queue;

int main ();




#endif