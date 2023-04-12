#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

# define MAX_LINE 80               // Maximum length of user input
# define MAX_ARGS MAX_LINE / 2 + 1 // Maximum number of arguments for a command
# define READ_END 0                // File descriptor for read end of a pipe
# define WRITE_END 1               // File descriptor for write end of a pipe
# define MAX_HISTORY 100          
	// Maximum number of commands to be stored in history

extern char	**environ;

typedef struct s_env
{
	char *env_var;      // environment variable name
	struct s_env *next; // next variable
}	t_env;

typedef struct s_history
{
	char *history[MAX_HISTORY]; // Array to store history of commands
	int history_count;          // Current count of commands in history
}	t_history;

/*				Environment				*/
t_env	**env_init(char **env);
void	print_env_vars(t_env **env_arr);
t_env	*search_env_name(t_env **stack, char *var_to_find);
/*				Commands				*/
int		check_commands(char **av);
int		check_commands2(char **av);
void	handle_pipe(char **av, int status, int i);
void	handle_redirection(char **av);
void	execute_command(char **av);

#endif