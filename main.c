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
			start = j;
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
	t_history cmd_history = {0}; // Initialize CommandHistory struct
	t_env **env_vars;
	int i;
	char *line;

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
		line = readline("\033[0;93m Minishell>$ \033[0;39m");
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

		// Check for built-in commands
		if (av[0] == NULL)
			continue ;
		else if (strcmp(av[0], "exit") == 0)
		{
			should_run = 0;
			break ;
		}
		//add_history(input);
		if (strcmp(av[0], "env") == 0)
		{
			print_env_vars(env_vars);
			continue ;
		}
		// We can add code for commands HERE!!!!!
		if (check_commands(av, env_vars))
			continue ;
		// Fork a child process to execute the command
		//execute_command(av);
	}
	return (0);
}
