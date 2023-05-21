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
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_LINE 80               // Maximum length of user input
# define MAX_ARGS MAX_LINE / 2 + 1 // Maximum number of arguments for a command
# define READ_END 0                // File descriptor for read end of a pipe
# define WRITE_END 1               // File descriptor for write end of a pipe
# define MAX_HISTORY 100          
	// Maximum number of commands to be stored in history

extern char	**environ;

typedef struct s_env
{
	char *env_name;
	char *env_value;
	struct s_env *next; // next variable
}	t_env;

typedef struct s_history
{
	char *history[MAX_HISTORY]; // Array to store history of commands
	int history_count;          // Current count of commands in history
}	t_history;


typedef struct s_arg
{
	char			*name;
	int				arg_len;
	int				in_quotes;
	char			quote_type;
	char			arg_type;
	struct s_arg 	*next;
}					t_arg;

/*				Environment				*/
t_env	**env_init(char **env);
t_env	*env_new(char *env_var);
void	env_add_back(t_env **env, t_env *new);
void	print_export_vars(t_env **env_arr);
void	print_env_vars(t_env **env_arr);
t_env	*search_env_name(t_env **stack, char *var_to_find);
char	*ft_strncpy(char *s1, char *s2, int n);
void	parse(char *res, char *str, char sep, int slen);
/*				Commands				*/
int		check_commands(char **av, t_env **child_env_vars);
int		check_commands2(char **av, t_env **child_env_vars);
void	handle_pipe(char **av, int status, int i);
void	handle_redirection(char **av);
void	execute_command(char **av, t_env **env_vars);
void	add_to_history(t_history *cmd_history, char *input);
void	print_history(t_history *cmd_history);
void	export_variable(t_env **env_vars, char *new_var);
void	unset_variable(t_env **env_vars, const char *var_name);
void	echo_command(char **av);
void	cd_command(char **av, t_env **child_env_vars);
void	execute_external_command(char **av, t_env **env_vars);
char	**env_vars_to_char_arr(t_env **env_vars);
void	free_double_array(char **doubles);
char	*get_env_value(char *name, t_env **env_vars);
void 	run_signals(int sig, int *ret_number);
int 	find_pipe(char **av);
void	setup_signals();
t_arg	**parse_arguments(char *string);
int		ft_argsize(t_arg *arg);
int		is_whitespace(char c);
int		quote_type(char c);


#endif