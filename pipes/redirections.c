/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 11:11:12 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/19 18:17:36 by pedperei         ###   ########.fr       */
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

/* void	handle_heredoc_redirection(char **av, int *j)
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
} */

/* void handle_heredoc_redirection(char **av, int *j)
{
    char *delimiter;
    char *line = NULL;
    int input_fd;
    int original_stdin_fd;

    delimiter = av[*j + 1];
    input_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    while ((line = readline("heredoc> ")) != NULL)
    {
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(input_fd, line, strlen(line));
        write(input_fd, "\n", 1);
        free(line);
    }
    close(input_fd);
    original_stdin_fd = dup(STDIN_FILENO); 
    input_fd = open("/tmp/minishell_heredoc", O_RDONLY);
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    unlink("/tmp/minishell_heredoc");
    dup2(original_stdin_fd, STDIN_FILENO);
    close(original_stdin_fd);
}
 */

int	write_line(char *delimiter, int temp_fd)
{
	char	*line;

	line = readline("heredoc> ");
	if (line == NULL)
	{
		return (-1);
	}
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

void	restore_stdin(void)
{
	int	terminal_fd;

	terminal_fd = open("/dev/tty", O_RDWR);
	if (terminal_fd < 0)
	{
		perror("open failed");
	}
	dup2(terminal_fd, STDIN_FILENO);
	close(terminal_fd);
}

void	handle_heredoc_redirection(char **av, int *j)
{
	char	*temp_file;
	int		temp_fd;
	pid_t	pid;
	char	*delimiter;

    delimiter = av[*j + 1];
	if (delimiter != NULL)
	{
		g_check_exit[0] = 1;
		restore_stdin();
		temp_file = "./minishell_heredoc";
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (temp_fd < 0)
			{
				perror("open failed");
				return ;
			}
			int status;
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
		else
		{
			waitpid(pid, NULL, 0);
		}
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
