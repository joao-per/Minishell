/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/26 10:54:36 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

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
		if (is_special_char(str[i++]) && quotes_closed == 1)
			return (print_error_messages('S', 0));
	}
	if (quotes_closed == 0)
		return (print_error_messages('U', 0));
	return (quotes_closed);
}
