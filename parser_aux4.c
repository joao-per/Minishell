/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/31 21:19:48 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	is_special_char(char c)
{
	if (c == '`' || c == '(' || c == ')' || c == '{' || c == '}')
		return (1);
	if (c == '&' || c == ';' || c == '*')
		return (1);
	if (c == '#' || c == '\\')
		return (1);
	return (0);
}

int	is_pipe_reds(char *input)
{
	if (ft_strcmp(input, ">") == 0 || ft_strcmp(input, ">>") == 0)
		return (1);
	if (ft_strcmp(input, "<") == 0 || ft_strcmp(input, "<<") == 0)
		return (1);
	if (ft_strcmp(input, "|") == 0)
		return (1);
	return (0);
}

int	is_pipe_reds_valid(t_arg **args)
{
	t_arg	*temp;

	temp = *args;
	while (temp)
	{
		if (is_pipe_reds(temp->name) && temp->quotes_perm == 0
			&& temp->next == NULL)
			return (print_error_messages('P', temp->name));
		if (ft_strcmp(temp->name, "||") == 0 && temp->quotes_perm == 0)
			return (print_error_messages('P', temp->name));
		if (temp->next && is_pipe_reds(temp->name) && temp->quotes_perm == 0
			&& is_pipe_reds(temp->next->name) && temp->quotes_perm == 0)
			return (print_error_messages('P', temp->name));
		temp = temp->next;
	}
	return (1);
}