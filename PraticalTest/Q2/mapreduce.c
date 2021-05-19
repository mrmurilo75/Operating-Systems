#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int parseIntFromFile(FILE*);	// read from file while isdigit and return int value
int parseIntFromString(char[]);	// ''

int main(int argc, char* argv[]) {
	if(argc != 4){
		fprintf(stderr, "wrong number of arguments\n");
		return 0;
	}

//	printf("\targc= %d\targv1= %s\targv2= %s\targv3= %s\t\n", argc, argv[1], argv[2], argv[3]);

	FILE* file = fopen(argv[1], "r");
	int n = parseIntFromFile(file),
		matriz[n][n];
	for(int j = 0; j < n; j++)
		for(int e = 0; e < n; e++)
			matriz[j][e] = parseIntFromFile(file);

	pid_t pid;
	int m = parseIntFromString(argv[2]),
		k = parseIntFromString(argv[3]),
		pipes[m][2],
		i,
		result;

//	printf("\tn= %d\tm= %d\tk= %d\n", n, m, k);

	for(i = 0; i < m; i++) {

		if (pipe(pipes[i]) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}

		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}

		if(pid == 0) break; 	// child breaks out

	}

	if(pid == 0) {	// child processing

//		printf("child= %d\ti= %d\n", pid, i);

		close(pipes[i][READ_END]);
		result = 0;

		for(int j = 0; j < n; j++) {

//			printf("\tj%%m= %d\n", j%m);

			if(j % m == i)
				for(int e = 0; e < n; e++)
					if(matriz[j][e] > k) {
//						printf("\telem= %d\n", matriz[j][e]);
						result++;
					}
		}

//		printf("\tresult= %d\n", result);

		if(write(pipes[i][WRITE_END], &result, sizeof(int)*sizeof(char)) < 0) {
			perror("Unable to write to pipe");
			exit(EXIT_FAILURE);
		}

		close(pipes[i][WRITE_END]);
		exit(EXIT_SUCCESS);
	}

	// parent processing

	int nbytes, current = 0;

	for(i = 0, result = 0; i < m; i++) {
		close(pipes[i][WRITE_END]);

		if ((nbytes = read(pipes[i][READ_END], &current, sizeof(int)*sizeof(char))) < 0 ) {
			fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(pipes[i][READ_END]);                 

		result+=current;
	}

	printf("%d\n", result);

	for(int i =0; i <m; i++)
		if(wait(NULL) < 0) {
			fprintf(stderr, "Unable to catch child exiting: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

	exit(EXIT_SUCCESS);
 
}


int parseIntFromFile(FILE* file) {
	if(file == NULL)
		return 0;

	char cur, buf[21];
	int i = 0;

	while(isdigit( cur = fgetc(file) ))
		buf[i++] = cur;
	buf[i] = '\0';

	if(i >= 21) {
		fprintf(stderr, "buffer overflown");
		exit(EXIT_FAILURE);
	}
	return parseIntFromString(buf);
}

int parseIntFromString(char* string) {
	int result = 0;

	for(int i = 0; string[i] != '\0' && isdigit( string[i] ); i++) {
		result *= 10;
		result += string[i] - '0';
	}

	return result;
}
