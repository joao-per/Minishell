#include "minishell.h"

void free_double_array(char **doubles)
{
	int i;

	i = 0;
	if (!doubles)
		return ;

	while (doubles[i] != NULL)
	{
		free(doubles[i]);
		i++;
	}
	free(doubles);
}

int find_pipe(char **av)
{
	int i = 0;
	while (av[i] != NULL)
	{
		if (ft_strcmp(av[i], "|") == 0)
			return i;
		i++;
	}
	return -1;
}