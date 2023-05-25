NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g #-fsanitize=address

FILES = main.c env_utils.c commands.c pipes.c utils.c parser.c signals.c redirections.c expansion.c free_mem_utils.c

all:
		$(MAKE) -C Libft all
		$(CC) $(FILES) $(CFLAGS) -lreadline Libft/libft.a -o $(NAME) 

fclean:
		rm -f $(NAME)
		$(MAKE) -C Libft fclean

re: fclean all