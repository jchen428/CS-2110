// Jesse Chen

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// This is the array into which you will load the raw data from the file
unsigned char data_arr[0x36 + 240 * 160 * 4];

int main(int argc, char *argv[]) {

	// 1. Make sure the user passed in the correct number of arguments
	if (argc != 2) {
		printf("Invalid number of arguments.\n");
		return -1;
	}

	// 2. Open the file; if it doesn't exist, tell the user and then exit
	FILE *fileIn = fopen(argv[1],  "r");
	if (fileIn == NULL) {
		printf("File not found.\n");
		return -1;
	}

	// 3. Read the file into the buffer then close it when you are done
	int bytesRead = fread(data_arr, 1, sizeof(data_arr), fileIn);

	//Compress pixel data to fit in GBA format
	for (int i = 0x36; i < bytesRead; i += 4) {
		data_arr[i] /= 8;
		data_arr[i + 1] /= 8;
		data_arr[i + 2] /= 8;
	}

	fclose(fileIn);

	// 4. Get the width and height of the image
	int width, height;
	int* pdata_arr = (int*) (data_arr + 0x12);

	width = *pdata_arr;
	pdata_arr = (int*) (data_arr + 0x16);
	height = *pdata_arr;

	// 5. Create header file, and write header contents; close it
	char* fileName = argv[1];
	char shortName[100];
	int length = strlen(fileName);

	//Remove file extension and replace with .h
	fileName[length - 4] = '\0';
	strcpy(shortName, fileName);
	strcat(fileName, ".h");

	FILE *headerFile = fopen(fileName, "w");

	//Copy the file name in caps
	length = strlen(shortName);
	char caps[length];
	for (int  i = 0; i <= length; i++) {
		caps[i] = toupper(shortName[i]);
	}

	//Write header contents
	fprintf(headerFile, "#define %s_WIDTH %i\n", caps, width);
	fprintf(headerFile, "#define %s_HEIGHT %i\n", caps, height);
	fprintf(headerFile, "const unsigned short %s_data[%i];", shortName, width * height);

	fclose(headerFile);

	// 6. Create c file, and write pixel data; close it
	//Remove file extension and replace with .c
	fileName[length] = '\0';
	strcat(fileName, ".c");
	FILE *dataFile = fopen(fileName, "w");

	//Make new array to store GBA pixel data in proper format
	int imgLength = width * height;
	int j = 0;
	unsigned short colorData[imgLength];
	
	//Concatenate color values
	for (int i = 0; i < imgLength; i++) {
		colorData[i] = (data_arr[j + 0x36] << 10) | (data_arr[j + 0x37] << 5) | data_arr[j + 0x38];
		j += 4;
	}

	fprintf(dataFile, "const unsigned short %s_data[%i] = {", shortName, width * height);

	//Write pixel data to file, accounting for BMP reverse row order
	int k = 0;
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			unsigned short gbaPixel = colorData[width * i + j];

			if (k % 10 == 0) {
				fprintf(dataFile, "\n\t");
			}
			fprintf(dataFile, "0x%04X, ", gbaPixel);
			k++;
		}
	}

	fprintf(dataFile, "\n};");

	fclose(dataFile);

	return 0;
}

