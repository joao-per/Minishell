#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include <signal.h>

# define MAX_LINE 80               // Maximum length of user input
# define MAX_ARGS MAX_LINE / 2 + 1 // Maximum number of arguments for a command
# define READ_END 0                // File descriptor for read end of a pipe
# define WRITE_END 1               // File descriptor for write end of a pipe
# define MAX_HISTORY 100
// Maximum number of commands to be stored in history

extern int	g_exit_status;

typedef struct s_env
{
	char	*env_name;
	char	*env_value;
	struct s_env	*next;
}	t_env;

typedef struct s_arg
{
	char	*name;
	int		arg_len;
	int		in_quotes;
	int		quotes_perm;
	char	quote_type;
	char	arg_type;
	struct s_arg	*next;
}	t_arg;

typedef struct s_shell
{
	char	**args_str;
	char	**envs_str;
	int		len_args;
	t_env	**envs;
	t_arg	**args;
	t_arg	**args_pipe;
	char	**args_str_pipe;
}	t_shell;

typedef struct s_clean_vars {
    t_shell *shell;
    t_env **env_vars;
    char **envs;
}   t_clean_vars;


/*				Environment				*/
int		ft_envsize(t_env *env);
t_env	*env_last(t_env *env);
void	env_add_back(t_env **env, t_env *new);
void	sep_name_value_env(t_env *new, char *env_var);
t_env	*env_new(char *env_var);
t_env	**env_init(char **env);
void	print_env_vars(t_env **env_arr);
void	print_export_vars(t_env **env_arr);
t_env	*search_env_name(t_env **stack, char *var_to_find);
char	*get_env_value(char *name, t_env **env_vars);
/*				Expansion				*/
void	free_expansion_aux(int *i, int *quotes_open, int *flag_type);
void	exp_quote_update(char c, int *flag_type, int *quotes_open);
char	*treat_expansion(char *input, t_env **env);
char	*expansion(char *input, char *str_exp, t_env **env, int *i);
/*				Parser	aux				*/
int		is_whitespace(char c);
int		is_pipes_red(char c);
int		quote_type(char c);
int		is_even_quotes(char *str);
int		is_special_char(char c);
int		is_pipe_reds_valid(t_arg **args);
/*				Parser args				*/
t_arg	*create_arg(t_arg **args);
t_arg	*arg_last(t_arg *arg);
void	arg_add_back(t_arg **arg, t_arg *new);
int		edit_parse_struct(t_arg *arg, int *i, char c, int quotes);
int		ft_argsize(t_arg *arg);
t_arg	**parse_arguments(char *string);
/*				Parser memory free		*/
void	cleanup_after_command(t_shell **shell);
void	free_extra_string_mem(t_arg **arg);
void	eliminate_extra_arg(t_arg **arg);
void	ft_argdelone(t_arg *arg);
void	ft_argclear(t_arg **arg);
/*				Parser					*/
int		parse_pipe_red(t_arg **args, int *i, char *sep);
int		parsing_tree(t_arg **args, int *i, char *str);
void	parse_aux(t_arg **args, t_arg *arg, char *str, int *i);
int		print_error_messages(char type_error, char *error);
t_arg	**parse_arguments(char *string);
/*				Shell Init				*/
t_shell	*shell_init(t_env **env_vars, t_arg **args, char **envs, char **av);
t_shell	*init_shell(t_env **env_vars, t_arg **args, char **envs);
char	**create_args_arr(t_arg **args);
char	**create_env_arr(t_env **envs);
/*				Commands				*/
void	cd_command(char **av, t_env **env_vars);
int		handle_cd(t_shell *shell);
int		handle_unset(t_shell *shell);
int		handle_export(t_shell *shell);
int		handle_pwd(t_shell *shell);
int		handle_echo(t_shell *shell);
int		handle_env(t_shell *shell);
void	delete_var(t_env **env_vars, const char *var_name);
void	validate_var_name(const char *var_name);
void	get_env_name_and_value(char **env_name, char **env_value, char *new_var);

/*				Free Memory Utils		*/
int		count_strings(char **str_arr);
void	free_env_array(char **str_arr);
void	free_args_array(char **str_arr, int len);
void	free_args_struct(t_arg **args);
void	free_env_struct(t_env **envs);
void	free_args(t_shell *shell, int len);
void	free_env(t_shell *shell);
/*				Signals					*/
void	restore_prompt(int sig);
void	back_slash(int sig);
void	setup_signals(void);
/*				Execute				*/
void	file_descriptor_handler(int in, int out);
void	handle_child_process(t_shell *shell, int in_fd, int out_fd, pid_t pid);
void	handle_pipe(t_shell *shell, int *in_fd, int pipe_index);
char	*construct_full_path(char *path_dir, char *command);
void	run_commands(t_shell *shell);
void	try_execve_at_each_path(t_shell *shell, char **path_dirs);
void	run_commands_aux(t_shell *shell, int in_fd, int out_fd);
void	execute_command(t_shell *shell);
void	execute_absolute_path(t_shell *shell);
void	execute_relative_path(t_shell *shell);
void	execute_external_command(t_shell *shell);
int		is_builtin_command(t_shell *shell);
/*				Redirections			*/
void	handle_redirection(t_shell *shell);
void	handle_append_redirection(char **av, int *j);
void	handle_output_redirection(char **av, int *j);
void	handle_heredoc_redirection(char **av, int *j);
void	handle_input_redirection(char **av, int *j);
/*				Built in Commands				*/
int		check_commands(t_shell *shell, pid_t pid);
int		check_commands2(t_shell *shell, pid_t pid);
void	export_variable(t_env **env_vars, char *new_var);
void	unset_variable(t_env **env_vars, const char *var_name);
void	echo_command(char **av);

void	free_double_array(char **doubles);
t_arg	*get_arg_byindex(t_shell *shell, int index);
char	*get_next_line(int fd);
void	create_args_pipe(t_shell *shell);

/*				Pipes				*/
int		find_pipe(t_shell *shell, int pipe_index);

#endif