/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 19:15:33 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

t_arg	*arg_last(t_arg *arg)
{
	if (!arg)
		return (0);
	while (arg->next)
		arg = arg->next;
	return (arg);
}

void	arg_add_back(t_arg **arg, t_arg *new)
{
	t_arg	*last;

	if (!new || !arg)
		return ;
	if (!*arg)
	{
		*arg = new;
		return ;
	}
	last = arg_last(*arg);
	last->next = new;
}

t_arg	*create_arg(t_arg **args)
{
	t_arg	*arg;

	arg = ft_calloc(1, sizeof(t_arg));
	if (!arg)
		return (0);
	arg->name = 0;
	arg->arg_len = 0;
	arg->in_quotes = 0;
	arg->quote_type = 0;
	arg_add_back(args, arg);
	return (arg);
}

int	edit_parse_struct(t_arg *arg, int *i, char c, int quotes)
{
	arg->in_quotes = quotes;
	arg->quote_type = c;
	(*i)++;
	return (1);
}

int	ft_argsize(t_arg *arg)
{
	t_arg		*temp;
	int			i;

	temp = arg;
	i = 1;
	if (arg == NULL)
		return (0);
	while (temp->next != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}
