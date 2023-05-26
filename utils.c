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

