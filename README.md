# `get_next_line`

- [📔 Project overview](#-project-overview)
- [📚 Concept guide](#-concept-guide)
- [🔗 Linked list implementation](#-linked-list-implementation)
- [💡 Potential improvements](#-potential-improvements)
- [📝 Notes](#-notes)
- [🛠️ Compilation and testing](#️-compilation-and-testing)
  
## 📔 Project overview
- **Objective**: Implement a function, `get_next_line`, that reads a file line by line from a given file descriptor. Repeatedly calling the function will read the file line by line until the end of the file is reached.
- **Function signature**: `char *get_next_line(int fd);` where `fd` is the file descriptor of the file to be read.
- **Authorised functions**: `read`, `malloc`, `free`.
- **Returns**: 
	- ✔️ The line read from the file descriptor.
	- ❌ `NULL` if there is nothing else to read or an error occurred.
- **Compilation flag**: The project requires the ability to be compiled with an optional flag to set a buffer size, e.g., `-D BUFFER_SIZE=32`.

### Learning objectives
- Deepen understanding of C programming, focusing on file I/O operations and memory management.
- Explore static variables and their utility in retaining state across function calls.
- Implement dynamic memory allocation strategies effectively.

#### Directives
- Make sure the function behaves well when it reads from a file and when it reads from the standard input.
- The returned line should include the `\n` character, except for the last line of the file if it doesn't end with a `\n`.
- The header file must contain the prototype of the function.
- Add needed functions to `get_next_line_utils.c`.
- It must compile with the flag `-D BUFFER_SIZE=xx`.
- `cc -Wall -Wextra -Werror -D BUFFER_SIZE=32 get_next_line.c get_next_line_utils.c`.
- `get_next_line` has an undefined behavior if the file to which the file descriptor corresponds has changed between calls, as long as `read()` hasn't reached the end of the file.
- `get_next_line` has an undefined behavior if reading from a binary file. However, you can implement a logical way to handle this.
#### Bonus
- Use a single static variable.
- Handle multiple file descriptors at the same time.

## 📚 Concept guide

1. **File descriptors**: A file descriptor is a unique integer identifier assigned by the operating system to an opened file, used for reading from or writing to that file.
2. **Static variables**: In C, a static variable retains its value between function calls. This is crucial for `get_next_line`, as it needs to remember the contents of the buffer between calls.
3. **Buffer management**: Reading from the file into a buffer requires understanding how to manage this buffer, including appending to it, finding the newline character, and removing the line from the buffer once it has been read.
4. **Memory allocation and management**: Dynamic memory allocation is necessary for the line being read. This involves being comfortable with `malloc` and `free`, understanding how to reallocate memory with the allowed functions if the line is longer than the buffer, and freeing memory when it is no longer needed.
5. **Error handling**: Correct error handling is essential, such as managing a failed `read` call or a failed `malloc` call.
6. **String manipulation**: Some string manipulation is required, like finding the newline character in a string, copying a substring, and concatenating strings.
7. **Edge cases**: Several edge cases must be considered, such as scenarios where the `fd` is invalid, the file ends without a newline character, it has very long lines, or is empty.
8. **Performance**: Ensuring the function's efficiency is important. This includes minimizing the number of `read` calls and avoiding unnecessary work.


## 🔗 Linked list implementation

This project was implemented using a linked list to store the buffer/fragments and file descriptor information. The linked list implementation not only allows for multiple file descriptors to be handled simultaneously but also simplifies the management of the buffer and file descriptor information and potentially improves performance as it avoids unnecessary memory reallocations.

### Linked list structure

```c
typedef struct s_list
{
	struct s_list	*next;
	char			frag[BUFFER_SIZE + 1];
	char			*current;
	ssize_t			nl;
}	t_list;
```
`next`: Pointer to the next node in the linked list.
`frag`: Buffer to store the read data from the file descriptor.
`current`: Tracker pointer to hold the current position within frag. 
`nl`: Index of the newline character within the buffer and flag; -1 if not found, -2 if `End Of File (EOF)` is reached.

### Main function
The function checks if there is a proper head node, an existing line within the list, and creates nodes until a line is found or `EOF` if needed.
Then returns the line read from the file descriptor or NULL if an error occurs, `EOF` is reached, or if the line is empty.
```c
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
```

### Linked list functions

##### `t_list	*gnl_check_head(t_list **head, int fd);`
Checks if the head node is valid, creates a new node if the list is empty, and frees the list if `EOF` is reached.

##### `t_list	*gnl_new_node(int fd);`
Creates a new node and reads data from the file descriptor into the node's buffer. Sets the current position tracker and newline index or `EOF`.

##### `void	gnl_update_list(t_list **head);`
Updates the list by freeing nodes without a newline, updating the last node if a newline is found, and freeing the node if no newline is found.


## 💡 Potential improvements

- **Optimizing memory/buffer management**: Dynamically allocating memory based on the size of the line read, at least for the last node, could improve memory usage as the current implementation allocates a fixed buffer size for each node.

- **Multipurpose functions**: Due to the restrictions of the project, some functions are designed to handle multiple tasks. Separating these functions into smaller, more focused functions could improve readability and maintainability. Eg: `nl` serves as both newline index and as a flag for `EOF`.

- **Error reporting**: While the current implementation returns `NULL` for errors or `EOF`, implementing a more detailed error reporting mechanism could provide more context on the failure's nature, such as distinguishing between a read error, memory allocation failure, or reaching `EOF`. 

## 📝 Notes

- **Only the pointer is static**: The pointer to the head of the linked list is static, not the linked list itself. This allows the linked list to be created and destroyed as needed while maintaining the head pointer between function calls.


## 🛠️ Compilation and testing

Three files are provided for testing the project at `/test_files`.

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c main.c
```
or, for the bonus part:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c
```

### Run

```bash
./a.out <file> <number of lines to read>
```

Bonus (main made for 3 files):
```bash
./a.out <file1> <file2> <file3> <number of lines to read>
```
