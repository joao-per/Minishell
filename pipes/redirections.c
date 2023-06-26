/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 11:11:12 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/26 10:30:44 by joao-per         ###   ########.fr       */
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
	char	*temp_file;
	pid_t	pid;
	char	*delimiter;

	delimiter = av[*j + 1];
	if (delimiter != NULL)
	{
		g_check_exit[0] = 1;
		restore_stdin();
		temp_file = "./minishell_heredoc";
		pid = create_new_process();
		if (pid == 0)
			setup_child(delimiter, temp_file);
		else
		{
			wait_for_child(pid);
			process_in_parent(temp_file);
		}
	}
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
