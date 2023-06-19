/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 22:13:13 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/19 22:13:14 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	process_in_child(char *delimiter, int temp_fd)
{
	int	status;
	while (1)
	{
		status = write_line(delimiter, temp_fd);
		if (status == -1 || status == 1)
			break ;
	}
	close(temp_fd);
	g_check_exit[1] = 0;
	exit(0);
}

void	wait_for_child(pid_t pid)
{
	waitpid(pid, NULL, 0);
}

void	process_in_parent(char *temp_file)
{
	int		input_fd;

	input_fd = open(temp_file, O_RDONLY);
	if (input_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	unlink(temp_file);
	g_check_exit[1] = 0;
}

pid_t create_new_process(void)
{
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(1);
	}
	return pid;
}

void setup_child(char *delimiter, char *temp_file)
{
	int temp_fd;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_DFL);
	open_temp_file(temp_file, &temp_fd);
	process_in_child(delimiter, temp_fd);
}