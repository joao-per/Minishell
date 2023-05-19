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

void	parse_input(char *input, char **av, const char *delimiter)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	start = -1;
	while (input[j] != '\0')
	{
		if (start == -1 && input[j] != *delimiter)
		{
			start = j;
			if (input[j + 1] == '\0')
			{
				j++;
				av[i++] = ft_strndup(input + start, j - start);
				start = -1;
			}
		}
		else if (start != -1 && (input[j] == *delimiter || input[j
					+ 1] == '\0'))
		{
			if (input[j + 1] == '\0')
				j++;
			av[i++] = ft_strndup(input + start, j - start);
			start = -1;
		}
		j++;
	}
	av[i] = NULL; // Add NULL terminator to the end of av array
}

int main(int ac, char **argv, char **env)
{
	char input[MAX_LINE]; // Buffer to store user input
	char *av[MAX_ARGS];   // Array to store parsed user input
	int should_run;       // Flag to indicate when to exit the loop
	char * pre_split;
	t_env **env_vars;
	int i;
	char *line;
	int ret_number = 0;

	(void)ac;
	(void)argv;
	(void)env;
	should_run = 1;
	env_vars = env_init(env);
	i = -1;
	while (++i < MAX_ARGS)
		av[i] = 0;
	i = -1;
	while (++i < MAX_LINE)
		input[i] = 0;
	parse_input(input, av, " ");
	while (should_run)
	{
		setup_signals();
		line = readline("\033[0;93m Minishell>$ \033[0;39m");
		pre_split = ft_calloc((ft_strlen(line) * 2 + 1), 1);
		//parse(pre_split, line, 0, 0);
		//printf("%s\n", pre_split);
		if (line == NULL)
		// End of file (e.g. user pressed Ctrl-D)
		{
			should_run = 0;
			break;
		}
		strncpy(input, line, MAX_LINE - 1);
		input[MAX_LINE - 1] = '\0'; // Ensure null terminator
		free(line); // Free memory allocated by readline


		// Parse the input into separate arguments
		parse_input(input, av, " ");
		//if (!check_commands(av, env_vars))
		// Check for built-in commands
		if (av[0] == NULL)
			continue ;
		else if (ft_strcmp(av[0], "exit") == 0)
		{
			should_run = 0;
			break ;
		}
		add_history(input);
		if (ft_strcmp(av[0], "env") == 0)
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