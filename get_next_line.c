#include "get_next_line.h"

/**
 * @brief creates a new node and saves read data from fd
 * - allocates node
 * - reads from fd into node->frag
 * - sets node->current (position tracker) and node->nl (newline index or EOF)
 * @param fd file descriptor
 * @return new node or NULL if error
*/
t_list	*gnl_new_node(int fd)
{
	t_list	*node;
	ssize_t	bytes_read;

	node = gnl_calloc(1, sizeof(t_list));
	if (!node)
		return (NULL);
	bytes_read = read(fd, node->frag, BUFFER_SIZE);
	if (bytes_read < 0)
		return (gnl_free_list(&node));
	if (bytes_read == 0)
		node->nl = -2;
	if (bytes_read > 0)
	{
		node->current = node->frag;
		node->nl = gnl_find_nl_idx(node->frag);
	}
	return (node);
}

/**
 * @brief updates node list
 * - frees nodes w/o newline
 * - updates node if newline found (current ptr if more data, free node if not)
 * @param head list head
*/
void	gnl_update_list(t_list **head)
{
	t_list	*temp;

	while (*head && (*head)->nl == -1)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
	if (*head && (*head)->nl > -1)
	{
		if ((*head)->current[(*head)->nl + 1])
		{
			(*head)->current = &(*head)->current[(*head)->nl + 1];
			(*head)->nl = gnl_find_nl_idx((*head)->current);
		}
		else
		{
			temp = *head;
			*head = (*head)->next;
			free(temp);
		}
	}
}

/**
 * @brief creates a line from list nodes
 * - calculates size of line and allocates memory
 * - fills line with list nodes (gnl_fill_line)
 * - updates list (gnl_update_list)
 * @param head list head
 * @return line or NULL if error
*/
char	*gnl_make_line(t_list **head)
{
	size_t	size;
	char	*line;
	t_list	*temp;

	size = 1;
	temp = *head;
	while (temp->nl < 0 && temp->next != NULL)
	{
		size += BUFFER_SIZE;
		temp = temp->next;
	}
	if (temp->nl > -1)
	{
		size += temp->nl + 1;
	}
	if (temp->nl < 0)
		size += gnl_strlen(temp->current);
	line = gnl_calloc(1, size);
	if (!line || size == 0)
		return (gnl_free_list(head));
	gnl_fill_line(line, *head, size);
	if (line)
		gnl_update_list(head);
	return (line);
}

/**
 * @brief checks list head
 * - valid `fd` and `BUFFER_SIZE` values 
 * - creates new node if list is empty
 * - frees list if `nl` is -2 (EOF) and returns NULL
 * @param head list head
 * @param fd file descriptor
 * @return list head
*/
t_list	*gnl_check_head(t_list **head, int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!*head)
	{
		*head = gnl_new_node(fd);
		if (!*head)
			return (NULL);
	}
	if ((*head)->nl == -2)
		return (gnl_free_list(head));
	return (*head);
}

/**
 * @brief reads a line from a file descriptor
 * - checks for proper head (gnl_check_head)
 * - checks for existing line in list
 * - creates nodes until a line is found or EOF (while)
 * @param fd file descriptor
 * @return line read file descriptor or NULL if error, EOF or empty line
*/
char	*get_next_line(int fd)
{
	static t_list	*head;
	t_list			*current;
	char			*line;

	if (!gnl_check_head(&head, fd))
		return (NULL);
	if (head->nl > -1)
		return (gnl_make_line(&head));
	current = head;
	while (current && current->nl == -1)
	{
		current->next = gnl_new_node(fd);
		if (!current->next)
			return (gnl_free_list(&head));
		current = current->next;
		if (current->nl == -2)
		{
			line = gnl_make_line(&head);
			if (!line)
				return (gnl_free_list(&head));
			gnl_free_list(&head);
			return (line);
		}
	}
	return (gnl_make_line(&head));
}
