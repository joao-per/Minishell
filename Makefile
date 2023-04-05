NAME = minishell
CC = cc
CFLAGS = # -Wall -Werror -Wextra

FILES = main.c

all:
		$(MAKE) -C Libft all
		$(CC) $(CFLAGS) $(FILES) Libft/libft.a -o $(NAME)