#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h> // read, write, close
# include <stdlib.h> // malloc, free
# include <stdint.h> // SIZE_MAX (gnl_calloc)
# include <stdio.h>	// FOPEN_MAX (gnl bonus)

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef struct s_list
{
	struct s_list	*next;
	char			frag[BUFFER_SIZE + 1];
	char			*current;
	ssize_t			nl;
}	t_list;

char	*get_next_line(int fd);
void	*gnl_free_list(t_list **head);
void	*gnl_calloc(size_t count, size_t size);
void	gnl_fill_line(char *line, t_list *head, size_t size);
size_t	gnl_strlen(const char *s);
ssize_t	gnl_find_nl_idx(char *str);

#endif