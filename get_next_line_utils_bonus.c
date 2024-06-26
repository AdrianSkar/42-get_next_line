#include "get_next_line_bonus.h"

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

ssize_t	gnl_find_nl_idx(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief allocates memory and sets it to 0
 * - checks for overflow
 * - allocates memory
 * - sets all bytes to 0 for the whole block
 * @param nmemb number of elements
 * @param size size of each element
 * @return pointer to allocated memory or NULL if error
*/
void	*gnl_calloc(size_t nmemb, size_t size)
{
	void			*ptr;
	size_t			bytes_count;
	unsigned char	*ptr_c;

	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	bytes_count = (nmemb * size);
	ptr = malloc(bytes_count);
	if (ptr == NULL)
		return (NULL);
	ptr_c = (unsigned char *)ptr;
	while (bytes_count--)
		*ptr_c++ = 0;
	return (ptr);
}

/**
 * @brief fills line with data from list
 * - iterates through list while nodes have data and line is not full
 *	 - copies existing data while newline is not found
 *	 - if nl found, copies data and breaks
*/
void	gnl_fill_line(char *line, t_list *head, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (head && head->current && i < size)
	{
		j = 0;
		while (head->current[j] && head->nl < 0)
			line[i++] = head->current[j++];
		if (head->nl > -1)
		{
			while (head->current[j] && j <= (size_t)head->nl)
				line[i++] = head->current[j++];
			break ;
		}
		head = head->next;
	}
}

void	*gnl_free_list(t_list **head)
{
	t_list	*temp;

	while (*head)
	{
		temp = *head;
		*head = temp->next;
		free(temp);
	}
	return (NULL);
}
