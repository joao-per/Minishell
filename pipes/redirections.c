/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 11:11:12 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/06 22:53:27 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_input_redirection(char **av, int *j)
{
	char *input_file;
	int input_fd;

	input_file = av[*j + 1];
	input_fd = open(input_file, O_RDONLY);
	if (input_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

void handle_heredoc_redirection(char **av, int *j)
{
	char	*delimiter;
	char	*line = NULL;
	int		input_fd;

	delimiter = av[*j + 1];
	input_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(input_fd, line, strlen(line));
		write(input_fd, "\n", 1);  // manually add the newline character
		free(line);
	}
	close(input_fd);
	input_fd = open("/tmp/minishell_heredoc", O_RDONLY);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	av[*j] = NULL;
}


void handle_output_redirection(char **av, int *j)
{
	char *output_file;
	int output_fd;

	output_file = av[*j + 1];
	//printf("output_file: %s\n", output_file);
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
}

void handle_append_redirection(char **av, int *j)
{
	char *output_file;
	int output_fd;

	output_file = av[*j + 1];
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
}

//"cd" ".." "|" "pwd" ">" "a"
void handle_redirection(t_shell *shell)
{
	int j;
	t_arg *temp;

	j = 0;
	temp = get_arg_byindex(shell, shell->index);
	while (temp && strcmp(temp->name, "|") != 0)
	{
		//printf("temp->name: %s\n", temp->name);
		if (ft_strcmp(temp->name, "<") == 0 && temp->quotes_perm == 0)
			handle_input_redirection(shell->args_str, &j);
		else if (ft_strcmp(temp->name, "<<") == 0 && temp->quotes_perm == 0)
			handle_heredoc_redirection(shell->args_str, &j);
		else if (ft_strcmp(temp->name, ">") == 0 && temp->quotes_perm == 0)
		{
			//printf("Current arg: %s\n", shell->args_str[j]);
			handle_output_redirection(shell->args_str, &j);
		}
		else if (ft_strcmp(temp->name, ">>") == 0 && temp->quotes_perm == 0)
			handle_append_redirection(shell->args_str, &j);
		j++;
		temp = temp->next;
	}
}

