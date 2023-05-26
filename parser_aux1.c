/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 19:21:11 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"


int	is_whitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_pipes_red(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	quote_type(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '\"')
		return (2);
	return (0);
}

int	is_even_quotes(char *str)
{
	int	i;
	int	quotes_closed;
	int	flag_type;

	i = 0;
	quotes_closed = 1;
	while (str[i])
	{
		if (quote_type(str[i]))
		{
			if (quotes_closed == 1)
			{
				flag_type = quote_type(str[i]);
				quotes_closed = 0;
			}
			else if (quotes_closed == 0)
				if (flag_type == quote_type(str[i]))
					quotes_closed = 1;
		}
		i++;
	}
	if (quotes_closed == 0)
		printf("error: unclosed quotes\n");
	return (quotes_closed);
}

/* char	*del_quotes(char *str)
{
	int	i;
	int	j;
	int	quotes_open;
	int	flag_type;

	i = 0;
	j = 0;
	quotes_open = 0;
	while (str[i])
	{
		if (quote_type(str[i]))
		{
			if (quotes_open == 0)
			{
				flag_type = quote_type(str[i]);
				quotes_open = 1;
			}
			else if (quotes_open == 1)
				if (flag_type == quote_type(str[i]))
					quotes_open = 0;
		}
		if ((!quote_type(str[i]) && !quotes_open) || (quotes_open
				&& quote_type(str[i]) != flag_type))
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}
 */