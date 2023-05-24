/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:54:09 by joao-per          #+#    #+#             */
/*   Updated: 2023/04/07 14:54:09 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

char **create_args_arr(t_arg **args)
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

int main(int ac, char **argv, char **env)
{
	int should_run;       // Flag to indicate when to exit the loop
	t_env **env_vars;
	t_arg **args;
	int i;
	char *line;
	char *line_exp;
	char **av;
	int ret_number = 0;

	(void)ac;
	(void)argv;
	(void)env;
	should_run = 1;
	env_vars = env_init(env);
	while (should_run)
	{
		setup_signals();
		line = readline("\033[0;93m Minishell>$ \033[0;39m");
		if (line == NULL)
		// End of file (e.g. user pressed Ctrl-D)
		{
			should_run = 0;
			break;
		}
		line_exp = treat_expansion(line, env_vars);
		args = parse_arguments(line_exp);
		if (!args)
			continue;
		av = create_args_arr(args);
		i = -1;
		/* while (av[++i])
		{
			printf("%s\n", av[i]);
		} */
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
		execute_command(av, env_vars);
		// We can add code for commands HERE!!!!!
		//continue ;
		// Fork a child process to execute the command
	}
	return (0);
}
