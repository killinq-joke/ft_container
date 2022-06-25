SRCS	= main.cpp
CC		= clang++
# CFLAGS	= -Wall -Wextra -Werror
NAME	= container

.PHONY: all
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) && ./$(NAME)
