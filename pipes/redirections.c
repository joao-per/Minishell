/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 11:11:12 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/17 18:57:42 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_input_redirection(char **av, int *j)
{
	char	*input_file;
	int		input_fd;

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

void	handle_heredoc_redirection(char **av, int *j)
{
	char	*delimiter;
	char	*line;
	int		input_fd;

	line = NULL;
	delimiter = av[*j + 1];
	input_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(input_fd, line, strlen(line));
		write(input_fd, "\n", 1);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(input_fd);
	input_fd = open("/tmp/minishell_heredoc", O_RDONLY);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

void	handle_output_redirection(char **av, int *j)
{
	char	*output_file;
	int		output_fd;

	output_file = av[*j + 1];
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
}

void	handle_append_redirection(char **av, int *j)
{
	char	*output_file;
	int		output_fd;

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

void	handle_redirection(t_shell *shell)
{
	int		j;
	t_arg	*temp;

	j = 0;
	temp = get_arg_byindex(shell, shell->index);
	while (temp && strcmp(temp->name, "|") != 0)
	{
		if (ft_strcmp(temp->name, "<") == 0 && temp->quotes_perm == 0)
			handle_input_redirection(shell->args_str, &j);
		else if (ft_strcmp(temp->name, "<<") == 0 && temp->quotes_perm == 0)
			handle_heredoc_redirection(shell->args_str, &j);
		else if (ft_strcmp(temp->name, ">") == 0 && temp->quotes_perm == 0)
			handle_output_redirection(shell->args_str, &j);
		else if (ft_strcmp(temp->name, ">>") == 0 && temp->quotes_perm == 0)
			handle_append_redirection(shell->args_str, &j);
		j++;
		temp = temp->next;
	}
}
