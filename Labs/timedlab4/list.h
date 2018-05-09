#ifndef LIST_H
#define LIST_H

typedef struct lnode {
	struct lnode* next;
	int data;
} NODE;

typedef struct llist {
	NODE* head;
	NODE* tail;
	int size;
} LIST;

LIST* create_list();
NODE* create_node(int data);
void push_front(LIST *llist, int data);
int peek_index(LIST *llist, int index);
int pop_back(LIST *llist);
void print_list(LIST *list);

#endif
