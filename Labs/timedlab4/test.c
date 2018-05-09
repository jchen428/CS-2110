#include "list.h"
#include <stdio.h>
#include <stdlib.h>

// Jesse Chen

// Test your list code here! Make sure you test all the cases!
int main()
{
	LIST* llist = create_list();

	push_front(llist, 1);

	// Your tests here
	// Push stuff
	push_front(llist, 2);
	push_front(llist, 3);
	push_front(llist, 4);
	push_front(llist, 5);
	print_list(llist);
	printf("Size = %d\n\n", llist->size);

	// Try to push existing data
	push_front(llist, 1);
	push_front(llist, 1);
	push_front(llist, 2);
	print_list(llist);
	printf("Size = %d\n\n", llist->size);

	// Peek index -1, 0, 3, 10
	printf("Index -1 = %d\n", peek_index(llist, -1));
	printf("Index 0 = %d\n", peek_index(llist, 0));
	printf("Index 3 = %d\n", peek_index(llist, 3));
	printf("Index 10 = %d\n", peek_index(llist, 10));

	// Pop stuff
	pop_back(llist);
	pop_back(llist);
	/*pop_back(llist);
	pop_back(llist);
	pop_back(llist);
	print_list(llist);
	printf("Size = %d\n\n", llist->size);

	// Pop empty list
	pop_back(llist);
	print_list(llist);
	printf("Size = %d\n\n", llist->size);*/

	// Clean up
	free(llist);

	return 0;
}
