#include "../modules/queue.h"
#include <stdio.h>

void custem_printf(char *a){
    printf("%s\n", a);
}

int main(){
    Queue_t *queue = malloc(sizeof(Queue_t));
    queue->size = 0;
    queue->first = NULL;
    queue->last = NULL;
    char *a = "aa";
    char *b = "bb";
    char *c = "cc";

    printf_queue(queue, &custem_printf);
    put(queue, a);
    printf_queue(queue, &custem_printf);
    put(queue, b);
    printf_queue(queue, &custem_printf);
    put(queue, c);
    printf_queue(queue, &custem_printf);

    while(queue->size > 0){
        printf("%s\n", (char*)pop(queue));
    }

    free_queue(queue, NULL);

    return 0;
}
