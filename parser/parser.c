/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/26 10:54:54 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

t_arg	**free_parse_arguments_aux(char *string, int *i)
{
	if (i)
		free(i);
	if (string)
		free(string);
	return (0);
}

t_arg	**parse_arguments(char *string)
{
	int		*i;
	int		len;
	t_arg	**args;
	t_arg	*temp;

	i = 0;
	if (!is_even_quotes(string) || !string[0])
		return (free_parse_arguments_aux(string, i));
	len = ft_strlen(string);
	i = ft_calloc(1, sizeof(int));
	args = ft_calloc(1, sizeof(t_arg *));
	while (*i < len)
	{
		while (*i < len && is_whitespace(string[*i]))
			(*i)++;
		if (*i == len)
			break ;
		temp = create_arg(args);
		if (!temp)
			return (NULL);
		parse_aux(args, temp, string, i);
	}
	free_parse_arguments_aux(string, i);
	return (args);
}
