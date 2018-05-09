#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * This function will create a new list and return a pointer to that list.
 */
LIST* create_list()
{
	LIST* newList = (LIST*) malloc(sizeof(LIST));

	if (newList)
	{
		newList->head = NULL;
		newList->tail = NULL;
		newList->size = 0;
	}
	else
		printf("Memory allocation error");

	return newList;
}

/**
 * This function will create a node from the given data and return a pointer to that node.
 */
NODE* create_node(int data)
{
	NODE* newNode = (NODE*) malloc(sizeof(NODE));

	if (newNode)
	{
		newNode->next = NULL;
		newNode->data = data;
	}
	else
		printf("Memory allocation error");

	return newNode;
}

/**
 * This function will add the given data to the head of the linked list (llist), if the data is not already contained 
 * in the linked list.
 */
void push_front(LIST* llist, int data)
{
	for (int i = 0; i < llist->size; i++)
	{
		if (peek_index(llist, i) == data)
			return;
	}

	NODE* newHead = create_node(data);

	if (llist->head == NULL && llist->tail == NULL)
	{
		llist->head = newHead;
		llist->tail = newHead;
	}
	else
	{
		NODE* oldHead = llist->head;
    
	    newHead->next = oldHead;
	    llist->head = newHead;
	}

	llist->size++;
}

/**
 * This function will return the data located at the given index in the list (llist)
 * i.e. for a list of 5,7,8,4,9,3 peek_index(llist, 2) would give me the value 8.
 * If the index is out of bounds, return -1.
 */
int peek_index(LIST* llist, int index)
{
	if (index < 0 || index >= llist->size)
		return -1;

	NODE* curNode = llist->head;

	for (int i = 0; i < index; i++)
	{
		curNode = curNode->next;
	}

	return curNode->data;
}

/**
 * Removes the last node from the tail of the list and returns the data from the removed node to the user.
 * If the list is empty, return -1.
 */
int pop_back(LIST* llist)
{
	if (llist->size == 0 && llist->head == NULL)
		return -1;

	NODE* back = llist->tail;
	int retData = back->data;

	NODE* newTail = llist->head;
	for (int i = 0; i < llist->size - 1; i++)
	{
		newTail = newTail->next;
	}

	if (newTail == NULL)
	{
		llist->head = NULL;
		llist->tail = NULL;
	}
	else
	{
		newTail->next = NULL;
		llist->tail = newTail;
	}

	free(back);
	llist->size--;

	return retData;
}

/** 
 * This function will print out the entire list in a nicely formatted way. Do whatever you want, 
 * just try to make it easy to read.
 */
void print_list(LIST* llist)
{
	NODE* curNode = llist->head;

	for (int i = 0; i < llist->size; i++)
	{
		printf("%d\t", curNode->data);
		curNode = curNode->next;
	}
	printf("\n");
}
