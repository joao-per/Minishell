/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_aux5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 16:29:36 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

int	parse_pipe_red(t_arg **args, int *i, char *sep)
{
	t_arg	*temp;

	temp = create_arg(args);
	temp->name = ft_strdup(sep);
	(*i)++;
	return (1);
}

int	parsing_tree(t_arg **args, int *i, char *str)
{
	if (str[*i] == '|')
		return (parse_pipe_red(args, i, "|"));
	if (ft_strncmp(&str[*i], ">>", 2) == 0)
	{
		(*i)++;
		return (parse_pipe_red(args, i, ">>"));
	}
	if (ft_strncmp(&str[*i], "<<", 2) == 0)
	{
		(*i)++;
		return (parse_pipe_red(args, i, "<<"));
	}
	if (str[*i] == '>')
		return (parse_pipe_red(args, i, ">"));
	if (str[*i] == '<')
		return (parse_pipe_red(args, i, "<"));
	return (0);
}

void	handle_quote(t_arg *arg, int *i, char c, int *change)
{
	if (!arg->in_quotes && quote_type(c) && !*change)
	{
		*change = edit_parse_struct(arg, i, c, 1);
		arg->quotes_perm = 1;
	}
}

void	handle_end_of_quote(t_arg *arg, int *i, char c, int *change)
{
	if (arg->in_quotes && arg->quote_type == c && !*change)
		*change = edit_parse_struct(arg, i, 0, 0);
}

void	parse_aux(t_arg **args, t_arg *arg, char *str, int *i)
{
	char	c;
	int		change;

	arg->name = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	while (str[*i])
	{
		change = 0;
		c = str[*i];
		if (!arg->in_quotes && is_whitespace(c))
			break ;
		handle_quote(arg, i, c, &change);
		handle_end_of_quote(arg, i, c, &change);
		if (!arg->in_quotes && is_pipes_red(c))
		{
			if (ft_strcmp(arg->name, "") == 0)
				eliminate_extra_arg(args);
			change = parsing_tree(args, i, str);
			return ;
		}
		if (change)
			continue ;
		arg->name[arg->arg_len++] = c;
		(*i)++;
	}
}
