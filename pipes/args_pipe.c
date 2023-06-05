/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 18:59:57 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/05 22:42:18 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

char	*join_quotes(t_arg *temp)
{
	char	*string_quotes;
	char	*temp_str;

	if (temp->quotes_perm == 1)
	{
		temp_str = ft_strdup(temp->name);
		string_quotes = ft_strjoin("\"", temp_str);
		if (!string_quotes)
			return (0);
		free(temp_str);
		temp_str = string_quotes;
		string_quotes = ft_strjoin(string_quotes, "\"");
		if (!string_quotes)
			return (0);
		free(temp_str);
		temp_str = NULL;
		return (string_quotes);
	}
	else
		return (temp->name);
}

t_arg	*aux_pipe_loop(t_arg *temp, t_arg *temp_pipe, int *flag)
{
	char	*str_quotes;
	char	*temp_str;

	while (temp && ft_strcmp(temp->name, "|") != 0)
	{
		str_quotes = join_quotes(temp);
		if (*flag != 1 && temp->next && ft_strcmp(temp->next->name, "|") != 0)
			temp_pipe->name = ft_strjoin(str_quotes, " ");
		else
		{
			if (!temp_pipe->name)
				temp_pipe->name = ft_strdup("");
			temp_str = temp_pipe->name;
			temp_pipe->name = ft_strjoin(temp_pipe->name, str_quotes);
			free(temp_str);
		}
		if (temp->quotes_perm == 1)
			free(str_quotes);
		temp = temp->next;
		*flag = 1;
	}
	if (!temp_pipe->name)
		temp_pipe->name = ft_strdup("|");
	return (temp);
}

void	create_args_pipe(t_shell *shell)
{
	t_arg	*temp;
	t_arg	*temp_pipe;
	int		*flag;

	temp = *shell->args;
	shell->args_pipe = ft_calloc(1, sizeof(t_arg *));
	flag = ft_calloc(1, sizeof(int));
	if (!shell->args_pipe)
		return ;
	while (temp)
	{
		*flag = 0;
		if (temp)
			temp_pipe = create_arg(shell->args_pipe);
		temp = aux_pipe_loop(temp, temp_pipe, flag);
		if (*flag == 0)
			temp = temp->next;
	}
	shell->args_str_pipe = create_args_arr(shell->args_pipe);
	free(flag);
}
