#include "minishell.h"

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
	av[*j] = NULL;
}

void handle_heredoc_redirection(char **av, int *j)
{
	char *delimiter;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int input_fd;

	delimiter = av[*j + 1];
	input_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while ((read = getline(&line, &len, stdin)) != -1) {
		if (strcmp(line, delimiter) == 0) {
			break;
		}
		write(input_fd, line, read);
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
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
	av[*j] = NULL;
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
	av[*j] = NULL;
}

void handle_redirection(char **av)
{
	int j;

	j = 0;
	while (av[j] != NULL)
	{
		if (ft_strcmp(av[j], "<") == 0)
		{
			handle_input_redirection(av, &j);
			break ;
		}
		else if (ft_strcmp(av[j], "<<") == 0)
		{
			handle_heredoc_redirection(av, &j);
			break ;
		}
		else if (ft_strcmp(av[j], ">") == 0)
		{
			handle_output_redirection(av, &j);
			break ;
		}
		else if (ft_strcmp(av[j], ">>") == 0)
		{
			handle_append_redirection(av, &j);
			break ;
		}
		j++;
	}
}