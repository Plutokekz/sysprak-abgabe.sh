#include "queue.h"

void put(Queue_t *queue, void *data){
    Node_t *new_node = malloc(sizeof(Node_t));
    new_node->next = NULL;
    new_node->data = data;
    if(queue->size == 0){
        queue->first = new_node;
        queue->last = new_node;
    }else{
        queue->last->next = new_node;
        queue->last = new_node;
    }
    queue->size ++;
}

void *pop(Queue_t *queue){
    if (queue != NULL){
        if (queue->size <= 0){
            return NULL;
        }
        if (queue->first != NULL){
        queue->size--;
        Node_t *data = queue->first;
        queue->first = data->next;
        void *d = data->data;
        free(data);
        return d;
        }
    }
    return NULL;
}

void free_node(Node_t *node, void (*free_data)()){
    if (node != NULL){
        if (node->data != NULL && free_data != NULL){
            free_data(node->data);
        }if (node->next != NULL){
            free_node(node->next, free_data);
            free(node);
        }
    }
}

void free_queue(Queue_t *queue, void (*free_data)()){
    if (queue != NULL){
        if (queue->first != NULL){
            free_node(queue->first, free_data);
            queue->first = NULL;
        }if (queue->last != NULL){
            queue->last = NULL;
        }
        free(queue);
    }
}

void printf_node(Node_t *node, void (*printf_data)()){
    if (node != NULL){
        if (node->data != NULL){
            printf("    ");
            printf_data(node->data);
        }
        if (node->next != NULL){
            printf_node(node->next, printf_data);
        }
    }
}

void printf_queue(Queue_t *queue, void (*printf_data)()){
    printf("Queue{\n    size: %d,\n    data: ", queue->size);
    if (queue->first != NULL){
        printf_node(queue->first, printf_data);
    }
    printf("}\n");
}

