#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1
#define BUF_SIZE 256

int main(void) {

	pid_t pid;
	int pipes[2][2];

	for(int i = 0; i < 2; i++)
		if (pipe(pipes[i]) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(EXIT_FAILURE);
	}

	if(pid == 0) {	// child processing

		close(pipes[0][WRITE_END]);
		close(pipes[1][READ_END]);

		int len = 0;
		do {
			int nbytes;
			char line[BUF_SIZE];

			if ((nbytes = read(pipes[0][READ_END], line, BUF_SIZE)) < 0 ) {
					fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

			len = strlen(line);
			for(int i = 0; i < len; i++) {
				if(line[i] == EOF)
					line[i] = '\0';
				line[i] = toupper(line[i]);
			}

			if(write(pipes[1][WRITE_END], line, len) < 0) {
				perror("Unable to write to pipe");
				exit(EXIT_FAILURE);
			}

			printf("here");
		} while( len != BUF_SIZE - 1);

		close(pipes[0][READ_END]);
		close(pipes[1][WRITE_END]);
		exit(EXIT_SUCCESS);
	}

	// parent processing
	int nbytes;
	char line[BUF_SIZE];

	close(pipes[0][READ_END]);
	close(pipes[1][WRITE_END]);
	do {
		if(fgets(line, BUF_SIZE, stdin) == NULL) {
			line[1] = '\0';
			line[0] = EOF;
		}

		if(write(pipes[0][WRITE_END], line, BUF_SIZE) < 0) {
			perror("Unable to write to pipe");
			exit(EXIT_FAILURE);
		}

		if((nbytes = read(pipes[1][READ_END], line, BUF_SIZE)) < 0 ) {
			fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		printf("%s", line);
	} while(!feof(stdin));

	close(pipes[0][WRITE_END]);
	close(pipes[1][READ_END]);

	/* wait for child and exit */
	if(wait(NULL) < 0) {
		fprintf(stderr, "Unable to catch child exiting: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
 
}

