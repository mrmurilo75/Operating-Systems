#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int length;
	char* value;
} String;


int grep(FILE* file, String* toFind) {
	char read = 1;
	int line, col, cc = '\0', prev = cc;

	for(line = 1; cc != EOF; line++) {
		for(col = 1; cc != '\n' && cc != EOF; prev = cc, (read? cc = fgetc(file) : continue), col++ ) {
			read = 1;
			if(isspace(prev) && cc == toFind->value[0]) {
				read = 0;
				colCandidate = col;
				int i = 0;
				for(i = 1; i < toFind->length && (cc = fgetc(file)) == toFind->value; i++, col++);
				if(i == toFind->length && isspace(cc = fgetc(file)))
					prinf("[ %d: %d]\n", line, col);
			}
		}
	}

	return 0;
}

String* newString(char* value) {
	String* result = malloc(sizeof(String));

	result->length = strlen(value);
	result->value = value;

	return result;
}

int main(int argc, char*[] argv){
	if(argc != 3) {
		returnError(0); 	// wrong number of arguments

	FILE* file = fopen(argv[2], "r");
	if(file == NULL)
		returnError(1); 	// unable to open file

	String* toFind = newString(argv[1]);

	return grep(file, toFind);
}
