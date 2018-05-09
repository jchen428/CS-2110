#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Here we are going to write some functions to support a linked list that stores
 * Person data (name, age)
 */
typedef struct person_t
{
  char* name;
  int age;
} Person;

/* Example functions given to you. If you want to truly grasp these concepts, try
 * writing your own structs and functions as well!
 */

// Create a new Person
Person* create_person(const char* name, int age)
{
  Person* p = (Person*) malloc(sizeof(Person));
  p->name = strdup(name); // Uses malloc!
  p->age = age;
  return p;
}

// Make a deep copy of a Person
void* copy_person(const void* data)
{
  Person *p = (Person*) data;
  return create_person(p->name, p->age);
}

// Print a Person
void print_person(void* data)
{
  Person *p = (Person*) data;
  if (p != NULL)
    printf("%s, %d", p->name, p->age);
  printf("\n");
}

// Free a Person
void free_person(void* data)
{
  // This is safe because we should only be passing in Person struct pointers
  Person *p = (Person*) data;
  // free any malloc'd pointers contained in the Person struct (just name)
  free(p->name);
  // Now free the struct itself; this takes care of non-malloc'd data, like age.
  free(p);
}

// Return 1 if the person's name is 8+ characters long
int long_name(const void *data)
{
  Person *p = (Person*) data;
  return strlen(p->name) > 7;
}

// Return 1 if the person's name is < 8 characters long
int short_name(const void *data)
{
  Person *p = (Person*) data;
  return strlen(p->name) < 8;
}

/* This main function does a little testing
   Like all good CS Majors you should test
   your code here. There is no substitute for testing
   and you should be sure to test for all edge cases
   e.g., calling remove_front on an empty list.
*/
int main(void)
{
	/* Now to make use of all of this stuff */
	list* llist = create_list();

	/* What does an empty list contain?  Lets use our handy traversal function */
	printf("TEST CASE 1\nAn Empty list should print nothing here:\n");
	traverse(llist, print_person);
	printf("\n");

 	/* Lets add a person and then print */
 	push_front(llist, create_person("Andrew", 24));
 	printf("TEST CASE 2\nA List with one person should print that person:\n");
 	traverse(llist, print_person);
 	printf("\n");

 	/* Lets remove that person and then print */
 	remove_front(llist, free_person);
 	printf("TEST CASE 3\nAnother Empty list should print nothing here:\n");
 	traverse(llist, print_person);
 	printf("\n");

 	/* Lets add two people and then print */
 	push_front(llist, create_person("Nick", 22));
 	push_front(llist, create_person("Randal", 21));
 	printf("TEST CASE 4\nA List with two people should print those two people:\n");
 	traverse(llist, print_person);
 	printf("\n");

	/* Lets copy this list */
	list* llist2 = copy_list(llist, copy_person);
	printf("TEST CASE 5\nA copied list should print out the same two people:\n");
 	traverse(llist2, print_person);
 	printf("\n");

	/* Lets kill the list */
	empty_list(llist, free_person);
 	printf("TEST CASE 6\nAfter freeing all nodes the list should be empty:\n");
 	traverse(llist, print_person);
	printf("\n");

	/* Let's make a list of people, and remove certain ones! */
	/* Should remove anyone whose name is 8+ characters long */
	push_front(llist, create_person("Josephine", 27));
	push_front(llist, create_person("Dave", 34));
	push_front(llist, create_person("Benjamin", 23));
	push_front(llist, create_person("Lisa", 41));
	push_front(llist, create_person("Maximilian", 24));
	printf("TEST CASE 7\nShould only print 2 people with short names:\n");
  printf("Removed %d\n", remove_if(llist, long_name, free_person));
	traverse(llist, print_person);
  printf("\n");

 	/* YOU ARE REQUIRED TO MAKE MORE TEST CASES THAN THE ONES PROVIDED HERE */
 	/* You will get points off if you do not you should at least test each function here */
  empty_list(llist, free_person);
  empty_list(llist, free_person);
  printf("TEST CASE 8\nEmptying an empty list should print nothing:\n");
  traverse(llist, print_person);
  printf("\n");

  remove_back(llist, free_person);
  printf("TEST CASE 9\nRemoving from an empty list should print nothing:\n");
  traverse(llist, print_person);
  printf("\n");

  push_back(llist2, create_person("Josephine", 27));
  push_back(llist2, create_person("Dave", 34));
  push_back(llist2, create_person("Benjamin", 23));
  push_back(llist2, create_person("Lisa", 41));
  push_back(llist2, create_person("Maximilian", 24));
  printf("TEST CASE 10\nAdd to a copied array should print all people:\n");
  traverse(llist2, print_person);
  printf("\n");

  printf("TEST CASE 11\nShould print number of people in list (7):\n");
  printf("size = %d\n", size(llist2));
  printf("\n");

  printf("TEST CASE 12\nShould print list head:\n");
  print_person(front(llist2));
  printf("\n");

  printf("TEST CASE 13\nShould print list tail:\n");
  print_person(back(llist2));
  printf("\n");

  printf("TEST CASE 14\nShould only print 3 people with long names:\n");
  printf("Removed %d\n", remove_if(llist2, short_name, free_person));
  traverse(llist2, print_person);
  printf("\n");

  printf("TEST CASE 15\nShould print list head:\n");
  print_person(front(llist2));
  printf("\n");

  printf("TEST CASE 16\nShould print list tail:\n");
  print_person(back(llist2));
  printf("\n");

  remove_back(llist2, free_person);
  remove_front(llist2, free_person);
  printf("TEST CASE 17\nShould print the same person as head and tail:\n");
  printf("Head: ");
  print_person(front(llist2));
  printf("Tail: ");
  print_person(back(llist2));
  printf("\n");

  printf("TEST CASE 18\nOne person remaining should print 0:\n");
  printf("is_empty = %d\n", is_empty(llist2));
  printf("\n");

  remove_front(llist2, free_person);
  printf("TEST CASE 19\nShould print nothing as head and tail:\n");
  printf("Head: ");
  print_person(front(llist2));
  printf("Tail: ");
  print_person(back(llist2));
  printf("\n");

  printf("TEST CASE 20\nNo one remaining should print 1:\n");
  printf("is_empty = %d\n", is_empty(llist2));
  printf("\n");

  list* llist3 = copy_list(llist2, copy_person);
  printf("TEST CASE 21\nA copy of an empty list should print nothing:\n");
  traverse(llist3, print_person);
  printf("\n");

  printf("TEST CASE 22\nremove_if from empty list should print nothing:\n");
  printf("Removed %d\n", remove_if(llist3, short_name, free_person));
  traverse(llist3, print_person);
  printf("\n");

 	/* Testing over clean up*/
	empty_list(llist, free_person);
 	free(llist);
	empty_list(llist2, free_person);
	free(llist2);
  empty_list(llist3, free_person);
  free(llist3);

  return 0;
}
