typedef struct LinkedList {
	void *data;
	struct LinkedList *next;
} LinkedList;
void push(LinkedList **head, void *data, int data_size);
int searchLinkedList(LinkedList *from, void *value, int (*cmp)(void *a, void *b));
int deleteNodeAfter(LinkedList **prevNode);
LinkedList getByNameNode(LinkedList *start, void *value, int (*cmp)(void *a, void *b));
