/**  @file queue.h
  *
  *  @brief Generic Queue.
  *
  * Queue Struct, put, pop, free, print
  *
  * @bug not known
  */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
    void *data;
    struct Node *next;
}Node_t;

typedef struct Queue{
    int size;
    Node_t *first;
    Node_t *last;
}Queue_t;


/** @brief Queue an item in the queue
  *
  * @param queue
  * @param data void pointer for custem data
  *
  * queues the given pointer in the Queue
  *
  */
void put(Queue_t *queue, void *data);


/** @brief removes the first item from the queue
  *
  * @param queue
  *
  * removes the first item from the given queue pointer
  *
  */
void *pop(Queue_t *queue);


/** @brief free a queue node
  *
  * @param node
  * @param free_data a function to free the data pointer, NULL is possible if there is nothing to free.
  *
  * free's the the queue node and the data with the given
  * free_data function.
  * checks if the node has a next node, if there is a next node
  * it runs free_node on the next node.
  *
  */
void free_node(Node_t *node, void (*free_data)());


/** @brief free a queue
  *
  * @param queue
  * @param free_data a function to free the data pointer, NULL is possible if there is nothing to free.
  *
  * free's a queue and it's data nodes.
  *
  */
void free_queue(Queue_t *queue, void (*free_data)());


/** @brief print a queue
  *
  * @param queue
  * @param printf_data a funtion to print the data pointer, NULL is possible if you dont want to print anything.
  *
  * prints the givene queue and the data of the queue with the given printf_data function.
  *
  */
void printf_queue(Queue_t *queue, void (*printf_data)());

#endif
