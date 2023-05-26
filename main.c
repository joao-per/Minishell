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

char	**create_env_arr(t_env **envs)
{
	char **env;
	char *temp_str;
	t_env *temp;
	int i;

	temp = (*envs);
	i = 0;
	env = (char **)ft_calloc(ft_envsize(*envs) + 1, sizeof(char *));
	while (temp)
	{
		temp_str = ft_charjoin(temp->env_name, '=');
		env[i] = ft_strjoin(temp_str, temp->env_value);
		free(temp_str);
		temp = temp->next;
		i++;
	}
	env[i] = 0;
	return (env);
}

char	**create_args_arr(t_arg **args)
{
	char **av;
	t_arg *temp;
	int i;

	temp = (*args);
	i = 0;
	av = (char **)ft_calloc(ft_argsize(*args) + 1, sizeof(char *));
	while (temp)
	{
		av[i] = ft_strdup(temp->name);
		temp = temp->next;
		i++;
	}
	av[i] = 0;
	return (av);
}

t_shell *shell_init(t_env **env_vars, t_arg **args, char **envs, char **av)
{
	t_shell *shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (0);
	shell->envs = env_vars;
	shell->args = args;
	shell->envs_str = envs;
	shell->args_str = av;

	return (shell);
}

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
	int len;

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
		args = parse_arguments(line_exp);
		free(line_exp);
		if (!args)
			continue ;
		av = create_args_arr(args);
		len = count_strings(av);
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
		add_history(line);
		free(line);
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
		free_args(shell, len);
		free(shell);
	}
	free_env(shell);
	free_args(shell, len);
	free(shell);
	free(line);
	rl_clear_history();
	return (0);
}
