/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/04/07 14:54:09 by joao-per          #+#    #+#             */
/*   Updated: 2023/04/07 14:54:09 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int exit_status;

int	main(int ac, char **argv, char **env)
{
	int should_run; // Flag to indicate when to exit the loop
	t_shell *shell;
	t_env **env_vars;
	t_arg **args;
	char *line;
	char *line_exp;
	char **av;
	char **envs;

	(void)ac;
	(void)argv;
	(void)env;
	env_vars = env_init(env);
	envs = create_env_arr(env_vars);
	should_run = 1;
	while (should_run)
	{
		setup_signals();
		line = readline("\033[0;93m Minishell>$ \033[0;39m");
		if (line == NULL)
		// End of file (e.g. user pressed Ctrl-D)
		{
			should_run = 0;
			break ;
		}
		line_exp = treat_expansion(line, env_vars);
		add_history(line);
		free(line);
		args = parse_arguments(line_exp);
		free(line_exp);
		if (!args)
			continue ;
		av = create_args_arr(args);
		if (!av)
			continue ;
		shell = shell_init(env_vars, args, envs, av);
		//if (!check_commands(av, env_vars))
		// Check for built-in commands
		if ((*args)->name == NULL)
			continue ;
		else if (ft_strcmp((*args)->name, "exit") == 0)
		{
			should_run = 0;
			break ;
		}
		if (ft_strcmp((*args)->name, "env") == 0)
		{
			print_env_vars(env_vars);
			continue ;
		}
		//check_commands(av, env_vars);
		execute_command(shell, env_vars);
		// We can add code for commands HERE!!!!!
		//continue ;
		// Fork a child process to execute the command
		free_args(shell, shell->len_args);
		free(shell);
	}
	free_env(shell);
	free_args(shell, shell->len_args);
	free(shell);
	free(line);
	rl_clear_history();
	return (0);
}
