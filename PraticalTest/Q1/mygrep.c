#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int length;
	char* value;
} String;


int grep(FILE* file, String* toFind) {
	int line, col, first, cc = '\0', prev = cc;

	for(line = 1; cc != EOF; line++, cc= fgetc(file)) {
		col = 1;
		first = 1;
		while(cc != EOF && cc != '\n') {

			if(isspace(prev) && cc == toFind->value[0]) {
				int colCandidate = col;
				int i = 1;

				for(i = 1; i < toFind->length && (cc = fgetc(file)) == toFind->value[i]; i++, col++);

				if(i == toFind->length && isspace( (cc = fgetc(file)) )) {
					col++;

					if(first) {
						printf("[ %d: ", line);
						first = 0;
					} else 
						printf(", ");
					printf("%d", colCandidate);
				}
			}

			prev = cc;
			if(cc != '\n')
				cc = fgetc(file);
			col++;
		}
		if(!first)
			printf("]\n");
	}

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

	FILE* file = fopen(argv[2], "r");

	if(file == NULL) {
		printf("unable to open file\n");
		return 2;
	}

	String* toFind = newString(argv[1]);

	file = fopen(argv[2], "r");

	return grep(file, toFind);
}
