NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g #-fsanitize=address

FILES = main.c commands.c env/env.c env/env_utils1.c shell_init.c \
		expansion.c free_mem_utils1.c free_mem_utils2.c error.c \
		parser/parser.c parser/parser_aux1.c parser/parser_aux2.c parser/parser_aux3.c parser/parser_aux4.c \
		pipes/pipes.c pipes/pipes_aux1.c pipes/pipes_aux2.c utils.c signals.c redirections.c \

all:
		$(MAKE) -C Libft all
		$(CC) $(FILES) $(CFLAGS) -lreadline Libft/libft.a -o $(NAME) 

fclean:
		rm -f $(NAME)
		$(MAKE) -C Libft fclean

re: fclean all