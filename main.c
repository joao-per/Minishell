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

int	*g_check_exit;

void	init_program(char **env, t_env ***env_vars, char ***envs)
{
	*env_vars = env_init(env);
	*envs = create_env_arr(*env_vars);
}

t_arg	**process_input(char **line, t_env **env_vars, char **envs)
{
	char	*line_exp;

	g_check_exit[1] = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, restore_prompt);
	*line = readline("\033[0;93mMinishell>$ \033[0;39m");
	signal(SIGQUIT, restore_prompt);
	if (*line == NULL)
	{
		printf("exit\n");
		if (env_vars)
			free_env_struct(env_vars);
		if (envs)
			free_env_array(envs);
		return (NULL);
	}
	line_exp = treat_expansion(*line, env_vars);
	add_history(*line);
	free(*line);
	return (parse_arguments(line_exp));
}

int	should_run_aux(t_shell *shell)
{
	int	should_run;

	should_run = check_exit(shell);
	if (should_run == 1)
	{
		execute_command(shell);
		cleanup_after_command(&shell);
		return (1);
	}
	else if (should_run == -1)
	{
		free_env(shell);
		cleanup_after_command(&shell);
		return (-1);
	}
	else if (should_run == 2)
	{
		cleanup_after_command(&shell);
		return (2);
	}
	return (0);
}

void	main_loop(t_env **env_vars, char **envs)
{
	int		should_run;
	t_shell	*shell;
	t_arg	**args;
	char	*line;

	shell = NULL;
	should_run = 1;
	while (should_run)
	{
		args = process_input(&line, env_vars, envs);
		if (!args)
		{
			if (!line)
				break ;
			continue ;
		}
		shell = init_shell(env_vars, args, envs);
		if (!shell)
			continue ;
		should_run = should_run_aux(shell);
		if (should_run == 2)
			continue ;
		if (should_run == -1)
			break ;
	}
}

int	main(int ac, char **argv, char **env)
{
	t_env	**env_vars;
	char	**envs;

	(void)ac;
	(void)argv;
	(void)env;
	g_check_exit = ft_calloc(2, sizeof(int));
	if (!g_check_exit)
		return (0);
	init_program(env, &env_vars, &envs);
	main_loop(env_vars, envs);
	return (0);
}
