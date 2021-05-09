#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int length;
	char* value;
} String;


int grep(FILE* file, String* toFind) {
	char read = 1;
	int line, col, cc = ' ', prev = cc;

	printf("\tbegin grep\n");

	for(line = 1; cc != EOF; line++) {
		printf("begin\n");
		for(col = 1; cc != EOF && cc != '\n'; prev = cc, (read? cc = fgetc(file) : 0), col++ ) {
			read = 1;
					printf("%c\n", cc);
			if(isspace(prev) && cc == toFind->value[0]) {
				read = 0;
				int colCandidate = col;
				int i = 0;
				for(i = 1; i < toFind->length && (cc = fgetc(file)) == toFind->value[i]; i++, col++);
				if(i == toFind->length && isspace(cc = fgetc(file)))
					printf("[ %d: %d]\n", line, colCandidate);
			}
		}
	}
	printf("\tend grep\n");

	return 0;
}

String* newString(char* value) {
	String* result = malloc(sizeof(String));

	result->length = strlen(value);
	result->value = value;

	return result;
}

int main(int argc, char** argv){
	if(argc != 3) {
		printf("wrong number of arguments\n");
		return 1;
	}

	printf("opening %s\n", argv[2]);

	FILE* file = fopen(argv[2], "r");

	printf("opened %s at %p\n", argv[2], file);

	if(file == NULL) {
		printf("unable to open file\n");
		return 2;
	}

	String* toFind = newString(argv[1]);

	printf("toFind.value = %s .length= %d\n", toFind->value, toFind->length);

	return grep(file, toFind);
}
