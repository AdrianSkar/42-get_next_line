CC = cc
BUFFER_SIZE ?= 4096 # Default buffer size
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE)
EXEC = a.out

# GNL
SRC_GNL = get_next_line.c get_next_line_utils.c main.c
OBJS = $(SRC_GNL:.c=.o)

# Bonus
SRC_BONUS = get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c
OBJS_BONUS = $(SRC_BONUS:.c=.o)

# Regular objects
$(OBJS): %.o: %.c get_next_line.h
	$(CC) $(CFLAGS) -c $< -o $@

# Bonus objects
$(OBJS_BONUS): %.o: %.c get_next_line_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) -o $(EXEC) $^

bonus: $(OBJS_BONUS)
	$(CC) -o $(EXEC) $^

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(EXEC) *.gch

re: fclean all

# make memcheck
TESTFILE = test2.txt
NUM_CALLS = 2
memcheck: $(EXEC)
	@valgrind --leak-check=full --show-leak-kinds=all -s --track-origins=yes ./$(EXEC) $(TESTFILE) $(NUM_CALLS)

.PHONY: all clean fclean bonus re memcheck