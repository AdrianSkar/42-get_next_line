#include "get_next_line.h"
#include <fcntl.h> // open
#include <stdio.h> // printf
#include <errno.h> // strerror
#include <fcntl.h> // open, O_RDONLY, O_WRONLY, O_RDWR, 

/// colored output
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"


int	main(int argc, char **argv)
{
	int	fd;
	int	num_lines;
	char	*line;
	int	i;
	
 	if (argc != 3)
	{
		printf("Usage: %s <file> <num_calls>\n", argv[0]);
		return (1);
	}
	//fd = 0;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return 1;
	}
	num_lines = atoi(argv[2]) ? atoi(argv[2]) : 3;
	for (i = 0; i < num_lines; i++)
	{
		line = get_next_line(fd);
		if (line)
			printf(GREEN "%d: %s\n" RESET, i + 1, line);
		else
			printf(RED "%d: NULL\n" RESET, i + 1);
		free(line);
	}
	close(fd);

	return (0);
}