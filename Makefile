NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g -fsanitize=address

FILES = main.c env_utils.c commands.c pipes.c utils.c parser.c signals.c redirections.c expansion.c

all:
		$(MAKE) -C Libft all
		$(CC) $(FILES) Libft/libft.a -o $(NAME) -lreadline

fclean:
		rm -f $(NAME)
		$(MAKE) -C Libft fclean

re: fclean all