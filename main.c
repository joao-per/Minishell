/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:58:45 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/01 23:58:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libft/libft.h"
#include "minishell.h"

int exit_status;

void init_program(char **env, t_env ***env_vars, char ***envs)
{
	setup_signals();
	*env_vars = env_init(env);
	*envs = create_env_arr(*env_vars);
}

t_arg **process_input(char **line, t_env ***env_vars)
{
	char *line_exp;
	t_arg **args;

	*line = readline("\033[0;93m Minishell>$ \033[0;39m");
	if (*line == NULL)
		return NULL;
	line_exp = treat_expansion(*line, *env_vars);
	add_history(*line);
	free(*line);
	args = parse_arguments(line_exp);
	free(line_exp);
	return (args);
}

t_shell *init_shell(t_env ***env_vars, t_arg **args, char ***envs)
{
	char **av;

	if (!args)
		return NULL;
	av = create_args_arr(args);
	if (!av)
		return NULL;
	return (shell_init(*env_vars, args, *envs, av));
}

int	check_command(t_shell *shell)
{
	if (shell->args && shell->args[0]->name)
	{
		if (ft_strcmp(shell->args[0]->name, "exit") == 0)
			return (0);
		if (ft_strcmp(shell->args[0]->name, "env") == 0)
		{
			print_env_vars(shell->envs);
			return (-1);
		}
	}
	return (1);
}

void	cleanup_after_command(t_shell *shell)
{
	free_args(shell, shell->len_args);
	free(shell);
}

void	main_loop(t_env **env_vars, char **envs)
{
	int should_run;
	t_shell *shell;
	t_arg **args;
	char *line;

	should_run = 1;
	while (should_run)
	{
		args = process_input(&line, &env_vars);
		if (!args)
			continue;
		shell = init_shell(&env_vars, args, &envs);
		if (!shell)
			continue;
		should_run = check_command(shell);
		if (should_run == 1)
		{
			execute_command(shell);
			cleanup_after_command(shell);
		}
	}
	free_env(shell);
	free_args(shell, shell->len_args);
	free(shell);
	//free(line);
	rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
    t_env **env_vars;
    char **envs;

    (void)ac;
    (void)av;
    (void)env;
    init_program(env, &env_vars, &envs);
    main_loop(env_vars, envs);

    return (0);
}
