//Jesse Chen

#include "my_malloc.h"

/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you will receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* make sure this always points to the beginning of your current
 * heap space! if it does not, then the grader will not be able
 * to run correctly and you will receive 0 credit. remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it
 * however, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;

/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/**** SIZES FOR THE FREE LIST ****
 * freelist[0] -> 16
 * freelist[1] -> 32
 * freelist[2] -> 64
 * freelist[3] -> 128
 * freelist[4] -> 256
 * freelist[5] -> 512
 * freelist[6] -> 1024
 * freelist[7] -> 2048
 */

int getMinIndex(int s);
metadata_t* getBlock(int i, int j);
metadata_t* getBuddy(metadata_t* freeBlock);

void* my_malloc(size_t size)
{
  if (size <= 0)
    return NULL;

  int actualSize = size + sizeof(metadata_t);	//Actual memory size needed

  //Return NULL if requested more than 2 KB
  if (actualSize > 2048) {
  	ERRNO = SINGLE_REQUEST_TOO_LARGE;
  	return NULL;
  }

	//Initialize heap if first call
  if (heap == NULL) {
  	heap = my_sbrk(SBRK_SIZE);
  	freelist[7] = (metadata_t*) heap;
  	freelist[7]->in_use = 0;
  	freelist[7]->size = 2048;
  	freelist[7]->prev = NULL;
    freelist[7]->next = NULL;

  	if (heap == NULL) {    //Return NULL if sbrk fails since heap is still NULL
      ERRNO = OUT_OF_MEMORY;
  		return NULL;
    }
  }

  int i = getMinIndex(actualSize);		//Find appropriate index in freelist

  //Check if there is already a free block of the required size
  if (freelist[i] != NULL) {
    //Head and next pointers for current list in freelist
  	metadata_t* head = freelist[i];
  	metadata_t* next = head->next;

  	//Remove head
  	head->in_use = 1;
  	head->next = NULL;
  	head->prev = NULL;

  	//Set next as new head for the list if there is more than one free block
  	//Otherwise, set list to NULL
  	if (next != NULL) {
  		next->prev = NULL;
  		freelist[i] = next;
  	} else {
  		freelist[i] = NULL;
  	}

    ERRNO = NO_ERROR;
  	return (char*) head + sizeof(metadata_t);
  }

  //Find next biggest memory block to split in freelist
  int j = i + 1;		//Index of next biggest block
  while (j < 8 && freelist[j] == NULL)
  	j++;

  //Get more memory from sbrk if couldn't find any more free blocks
  if (j > 7) {
    metadata_t* newBlock = my_sbrk(SBRK_SIZE);
    if (newBlock == NULL) {   //Return NULL if sbrk fails
      ERRNO = OUT_OF_MEMORY;
      return NULL;
    }

    //Initialize newBlock's values
    newBlock->in_use = 0;
    newBlock->size = 2048;
    newBlock->next = NULL;
    newBlock->prev = NULL;

    //Add newBlock to top of the empty freelist
    j = 7;
    freelist[j] = newBlock;
  }

  //Keep splitting the block until the size indices match
  metadata_t* retBlock = getBlock(i, j);

  //Set block after retBlock as new head of the list if there is more than one free block
  //Otherwise, set list to NULL
  if (retBlock->next != NULL) {
    freelist[i] = retBlock->next;
    freelist[i]->prev = NULL;
  } else {
    freelist[i] = NULL;
  }

  //Remove retBlock from the list and return its address
  retBlock->in_use = 1;
  retBlock->next = NULL;

  ERRNO = NO_ERROR;
  return (char*) retBlock + sizeof(metadata_t);
}

void* my_calloc(size_t num, size_t size)
{
  size_t total = num * size;
  void* p = my_malloc(total);

  if (p != NULL) {
    char* p1 = p;

    while (total > 0) {
      *p1 = 0;
      p1++;
      total--;
    }

    ERRNO = NO_ERROR;
  }

  return p;
}

void my_free(void* ptr)
{
  //Get the block to free and its buddy
  metadata_t* freeBlock = (metadata_t*) ((char*) ptr - sizeof(metadata_t));
  metadata_t* buddy = getBuddy(freeBlock);

  freeBlock->in_use = 0;

  //Merge buddies while buddy exists and is not in use
  while (buddy != NULL && buddy->in_use == 0 && buddy->size < 2048) {
    //Extract buddy and fix surrounding pointers
    if (buddy->prev != NULL && buddy->next != NULL) {
      buddy->prev->next = buddy->next;
      buddy->next->prev = buddy->prev;
    } else if (buddy->prev != NULL) {
      buddy->prev->next = NULL;
    } else if (buddy->next != NULL) {
      buddy->next->prev = NULL;
    } else {
      freelist[getMinIndex(buddy->size)] = NULL;
    }

    if (freeBlock > buddy)    //Use the block with lower memory
      freeBlock = buddy;

    //Merge blocks and keep looking for more buddies if sizes match
    freeBlock->size *= 2;
    buddy = getBuddy(freeBlock);
  }

  //Shift list to make room for freeBlock
  int i = getMinIndex(freeBlock->size);
  if (freelist[i] != NULL) {
    metadata_t* head = freelist[i];
    head->prev = NULL;
    freeBlock->next = head;
  }

  //Set freeBlock to head of freelist
  freelist[i] = freeBlock;

  ERRNO = NO_ERROR;
}

void* my_memmove(void* dest, const void* src, size_t num_bytes)
{
  char* d = (char*) dest;
  char* s = (char*) src;

  if (s > d) {
    for (int i = 0; i < num_bytes; i++)
      d[i] = s[i];
  } else if (s < d) {
    for (int i = num_bytes - 1; i > -1; i--)
      d[i] = s[i];
  }

  ERRNO = NO_ERROR;
  return dest;
}

/**
 * Find the lowest index in freelist that will fit a memory request of size s
 */
int getMinIndex(int s) {
	int i = 0;
	int size = 16;

	while (size < s) {
		size *= 2;
		i++;
	}

	return i;
}

/**
 * Use the buddy system to recursively split a block of size freelist[j] to
 * size freelist[i]
 */
metadata_t* getBlock(int i, int j) {
  metadata_t* currBlock = freelist[j];

  //Return address of the current block once it's the proper size
  if (i == j)
    return currBlock;

  currBlock->size /= 2;

  //Make the split block's buddy
  metadata_t* buddy = (metadata_t*) ((char*) currBlock + currBlock->size);
  buddy->size = currBlock->size;

  //Set next block as new head of the list if there is more than one free block
  //Otherwise, set list to NULL
  if (currBlock->next != NULL) {
    freelist[j] = currBlock->next;
    freelist[j]->prev = NULL;
  } else {
    freelist[j] = NULL;
  }

  //Set buddies' next/prev pointers to each other
  currBlock->next = buddy;
  buddy->prev = currBlock;

  //Decrement current size index and set the now-smaller current block as the head
  //for the smaller list
  j--;
  freelist[j] = currBlock;

  return getBlock(i, j);
}

/**
 * Finds the address of freeBlock's buddy
 */
metadata_t* getBuddy(metadata_t* freeBlock) {
  metadata_t* buddy = (metadata_t*) ((((uintptr_t) freeBlock - (uintptr_t) heap)
                        ^ freeBlock->size) + (uintptr_t) heap);

  if (freeBlock->size == buddy->size)
    return buddy;
  else
    return NULL;
}
