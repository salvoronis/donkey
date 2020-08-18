#include "./list.h"
#include <stdio.h>
#include <stdlib.h>

enum boolean {
	FALSE,
	TRUE
};

void pushArr(LinkedList **head, unsigned char num, int data_size, void *value, ...){
	LinkedList *tmp = (LinkedList*) malloc(sizeof(LinkedList));
	tmp->data = malloc(data_size);
	while (num--){
        	tmp->data = value;
        	tmp->next = (*head);
        	(*head) = tmp;
		value++;
	}
}

void push(LinkedList **head, void *value, int data_size){
	LinkedList *tmp = (LinkedList*) malloc(sizeof(LinkedList));
	tmp->data = malloc(data_size);
        tmp->data = value;
	tmp->next = (*head);
        (*head) = tmp;
}

int searchLinkedList(LinkedList *from, void *value, int (*cmp)(void *a, void *b)){
        while ((from != NULL)){
                if(cmp(value,from->data)){
                        return TRUE;
                }
                from = from->next;
        }
        return FALSE;
}

int deleteLinkedListAfter(LinkedList **prevNode){
        LinkedList *tmp = (*prevNode)->next;
        if ((*prevNode)->next->next != NULL){
                (*prevNode)->next = (*prevNode)->next->next;
                free(tmp);
                return TRUE;
        }
        return FALSE;
}


LinkedList *getByNameNode(LinkedList *start, void *value, int (*cmp)(void *a, void *b)){
        while (start != NULL) {
                if (cmp(value, start->data)){
                        return start;
                }
                start = start->next;
        }
        return start;
}

