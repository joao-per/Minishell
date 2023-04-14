#include "minishell.h"

void	add_to_history(t_history *cmd_history, char *input)
{
	int	i;

	i = 0;
	if (cmd_history->history_count < MAX_HISTORY)
	{
		cmd_history->history[cmd_history->history_count++] = ft_strdup(input);
	}
	else
	{
		free(cmd_history->history[0]);
		while (++i < MAX_HISTORY)
			cmd_history->history[i - 1] = cmd_history->history[i];
		cmd_history->history[MAX_HISTORY - 1] = ft_strdup(input);
	}
}

void print_history(void)
{
	HIST_ENTRY **hist_list = history_list();
	int i = 0;
	if (hist_list)
	{
		while (hist_list[i])
		{
			printf("%d %s\n", i + 1, hist_list[i]->line);
			i++;
		}
	}
}
