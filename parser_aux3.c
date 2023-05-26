/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 19:17:41 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void free_extra_string_mem (t_arg **arg)
{
	char	*temp;

	temp = ft_strdup((*arg)->name);
	free((*arg)->name);
	(*arg)->name = temp;
}


void	eliminate_extra_arg(t_arg **arg)
{
	t_arg	*temp;
	t_arg	*end;

	temp = (*arg);
	end = NULL;
	while (temp->next != NULL)
	{
		if (temp->next->next == NULL)
			end = temp;
		temp = temp->next;
	}
	free(temp->name);
	free(temp);
	if (end)
		end->next = NULL;
	else
		(*arg) = NULL;
}

void	ft_argdelone(t_arg *arg)
{
	if (!arg)
		return ;
	free(arg->name);
	free(arg);
}


void	ft_argclear(t_arg **arg)
{
	t_arg	*temp;

	if (!arg || !*arg)
		return ;
	while (*arg)
	{
		temp = (*arg)->next;
		ft_argdelone(*arg);
		*arg = temp;
	}
	free(arg);
	arg = 0;
}
