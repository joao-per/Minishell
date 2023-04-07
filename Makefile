NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g

FILES = main.c env_utils.c commands.c

all:
		$(MAKE) -C Libft all
		$(CC) $(CFLAGS) $(FILES) Libft/libft.a -o $(NAME)

fclean:
		rm -f $(NAME)

re: fclean all