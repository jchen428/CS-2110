#include <stdlib.h>
#include <stdio.h>

int compare(const void *a, const void *b)
{
	int *x = (int*) a;
	int *y = (int*) b;

	return (*y - *x);
}

int main(void)
{
	int data[] = {3, 9, 0, -12, 42, -3, 18};

	printf("Before: data = ");
	int i = 0;
	for (i = 0; i < 7; i++)
		printf("%d ", data[i]);
	printf("\n");

	qsort(&data, 7, sizeof(int), compare);

	printf("After:  data = ");
	for (i = 0; i < 7; i++)
		printf("%d ", data[i]);
	printf("\n");
}
