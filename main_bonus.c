#include "get_next_line_bonus.h"
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <errno.h> // strerror
#include <fcntl.h> // open, O_RDONLY, O_WRONLY, O_RDWR, 

/// colored output
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

void print_and_free_line(char *line, int fd_number, int call_number) {
	if (line)
		printf(GREEN "file%d %d: %s\n" RESET, fd_number, call_number, line);
	else
		printf(RED "file%d %d: NULL\n" RESET, fd_number, call_number);
	free(line);
}

int	main(int argc, char **argv)
{
	int	fd_1, fd_2, fd_3;
	int	num_lines;
	char	*line_1, *line_2, *line_3;
	int	i;
	
 	if (argc != 5)
	{
		printf("Usage: %s <file1> <file2> <file3> <num_calls>\n", argv[0]);
		return (1);
	}

	fd_1 = open(argv[1], O_RDONLY);
	fd_2 = open(argv[2], O_RDONLY);
	fd_3 = open(argv[3], O_RDONLY);
	if (fd_1 == -1 || fd_2 == -1 || fd_3 == -1)
	{
		perror("Error opening files");
		return 1;
	}

	num_lines = atoi(argv[4]) ? atoi(argv[4]) : 3;

	for (i = 0; i < num_lines; i++)
	{
		line_1 = get_next_line(fd_1);
		line_2 = get_next_line(fd_2);
		line_3 = get_next_line(fd_3);

		printf("Call number %d\n", i + 1);
		print_and_free_line(line_1, 1, i + 1);
		print_and_free_line(line_2, 2, i + 1);
		print_and_free_line(line_3, 3, i + 1);
	}

	close(fd_1);
	close(fd_2);
	close(fd_3);

	return (0);
}

////// Dynamic file number alternative (changes args order) ///////
/* 
int main(int argc, char **argv) {
	int num_lines;
	int *fds;
	char *line;
	int num_files = argc - 2; // First argument is num_lines, so total files = argc - 2

	if (argc < 3) {
		printf("Usage: %s <num_calls> <file1> <file2> ... <fileN>\n", argv[0]);
		return 1;
	}

	num_lines = atoi(argv[1]);
	fds = malloc(num_files * sizeof(int));
	if (!fds) {
		perror("Memory allocation failed");
		return 1;
	}

	// Open files and store their file descriptors
	for (int i = 0; i < num_files; i++) {
		fds[i] = open(argv[i + 2], O_RDONLY);
		if (fds[i] == -1) {
			perror("Error opening file");
			// Close any opened files before exiting
			for (int j = 0; j < i; j++) {
				close(fds[j]);
			}
			free(fds);
			return 1;
		}
	}

	// Read lines from each file
	for (int i = 0; i < num_lines; i++) {
		printf("Call number %d\n", i + 1);
		for (int j = 0; j < num_files; j++) {
			line = get_next_line(fds[j]);
			print_and_free_line(line, j + 1, i + 1);
		}
	}

	// Close all file descriptors
	for (int i = 0; i < num_files; i++) {
		close(fds[i]);
	}

	free(fds);
	return 0;
}
 */
