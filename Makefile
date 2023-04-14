NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g

FILES = main.c env_utils.c commands.c pipes.c

all:
		$(MAKE) -C Libft all
		$(CC) $(FILES) Libft/libft.a -o $(NAME) -lreadline -lhistory

fclean:
		rm -f $(NAME)

re: fclean all