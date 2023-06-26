/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 22:13:20 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/20 10:52:29 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_stdin(void)
{
	int	terminal_fd;

	terminal_fd = open("/dev/tty", O_RDWR);
	if (terminal_fd < 0)
		perror("open failed");
	dup2(terminal_fd, STDIN_FILENO);
	close(terminal_fd);
}

int	write_line(char *delimiter, int temp_fd)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return (-1);
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	else
	{
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
	}
	free(line);
	return (0);
}

void	open_temp_file(char *temp_file, int *temp_fd)
{
	*temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (*temp_fd < 0)
	{
		perror("open failed");
		exit(1);
	}
}
