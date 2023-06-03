/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/06/01 23:58:45 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/01 23:58:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int exit_status;

void	init_program(char **env, t_env ***env_vars, char ***envs)
{
	*env_vars = env_init(env);
	*envs = create_env_arr(*env_vars);
}

t_shell	*init_shell(t_env **env_vars, t_arg **args, char **envs)
{
	t_shell *shell;
	char **args_str;

	args_str = create_args_arr(args);
	if (!args_str)
		return (0);
	shell = shell_init(env_vars, args, envs, args_str);
	return (shell);
}

t_arg	**process_input(char **line, t_env **env_vars)
{
	char *line_exp;

	*line = readline("\033[0;93m Minishell>$ \033[0;39m");
	if (*line == NULL)
	{
		printf("exit\n");
		return (NULL);
	}
	line_exp = treat_expansion(*line, env_vars);
	add_history(*line);
	free(*line);
	return (parse_arguments(line_exp));
}

int	check_command(t_shell *shell)
{
	t_arg	*temp;

	temp = (*shell->args);
	if (!(shell->args))
		return (2);
	if (temp == NULL)
		return (2);
	else if (ft_strcmp(temp->name, "exit") == 0)
		return (-1);
	return (1);
}

void	cleanup_after_command(t_shell **shell)
{
	if (shell && *shell)
	{
		free_args(*shell, (*shell)->len_args);
		free(*shell);
		*shell = NULL;
	}
}

void	main_loop(t_env **env_vars, char **envs)
{
	int should_run;
	t_shell *shell;
	t_arg **args;
	char *line;

	shell = NULL;
	should_run = 1;
	while (should_run)
	{
		args = process_input(&line, env_vars);
		if (!args)
		{
			if (!line)
				break ;
			continue ;
		}
		shell = init_shell(env_vars, args, envs);
		if (!shell)
			continue ;
		should_run = check_command(shell);
		if (should_run == 1)
		{
			execute_command(shell);
			cleanup_after_command(&shell);
		}
		else if (should_run == -1)
			break ;
		else if (should_run == 2)
		{
			cleanup_after_command(&shell);
			continue ;
		}
	}
	if (shell)
		free_env(shell);
}


int	main(int ac, char **argv, char **env)
{
	t_env	**env_vars;
	char	**envs;

	(void)ac;
	(void)argv;
	(void)env;
	setup_signals();
	init_program(env, &env_vars, &envs);
	main_loop(env_vars, envs);

	return (0);
}
