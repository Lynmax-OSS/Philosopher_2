# 42-style Makefile
NAME     := philo
CC       := cc
CFLAGS   := -Wall -Wextra -Werror
SRCS     := main.c init.c routine.c monitor.c utils.c cleanup.c
OBJS     := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -pthread -o $(NAME) $(OBJS)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
