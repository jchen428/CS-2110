char* myString = "Hello, world! My name is ";
char* input = readLine() // <-- Returns line from prompt
char* result = (char*) malloc((strlen(myString) + strlen(input) + 1) * sizeOf(char));
											//+1 for null terminator (only for strings)
					//void* malloc(size) -> ALWAYS CHECK FOR NULL POINTER
if (!result) {		// <==> if (myString == 0) -> NULL == 0
	//Handle error
}
strcpy(result, myString);
strcpy(result + strlen(myString), input);



typedef struct {
	int x;
	int y;
} Block;
Block* myBlocks = (Block*) malloc(50 * sizeof(Block)); //where 50 can be arbitrary assigned at runtime
myBlocks[10].x = 3; // <==> *(myBlocks + 10).x = 3;
myBlocks[10].y = 5; // <==> *(myBlocks + 10).y = 5;

free(myBlocks); //free memory when done with varible. never use that variable again
		//void free(void*)