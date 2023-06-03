NAME = minishell
CC = cc
CFLAGS =  -Wall -Werror -Wextra -g #-fsanitize=address

FILES = main.c commands/commands.c env/env.c env/env_utils1.c utils/shell_init.c \
		utils/expansion.c free_mem/free_mem_utils1.c free_mem/free_mem_utils2.c utils/error.c \
		parser/parser.c parser/parser_aux1.c parser/parser_aux2.c parser/parser_aux3.c parser/parser_aux4.c \
		pipes/pipes.c pipes/pipes_aux1.c pipes/pipes_aux2.c utils/utils.c commands/signals.c pipes/redirections.c commands/commands2.c\

all:
		$(MAKE) -C Libft all
		$(CC) $(FILES) $(CFLAGS) -lreadline Libft/libft.a -o $(NAME) 

fclean:
		rm -f $(NAME)
		$(MAKE) -C Libft fclean

re: fclean all