/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 19:18:01 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

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
		if (!arg->in_quotes && quote_type(c) && !change)
		{
			change = edit_parse_struct(arg, i, c, 1);
			arg->quotes_perm = 1;
		}
		if (arg->in_quotes && arg->quote_type == c && !change)
			change = edit_parse_struct(arg, i, 0, 0);
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
	/* temp = ft_strdup(arg->name);
	free(arg->name);
	arg->name = temp; */
}

t_arg	**parse_arguments(char *string)
{
	int		*i;
	int		len;
	t_arg	**args;
	t_arg	*temp;

	if (!is_even_quotes(string) || !string[0])
		return (0);
	len = ft_strlen(string);
	i = ft_calloc(1, sizeof(int));
	args = ft_calloc(1, sizeof(t_arg *));
	while (*i < len)
	{
		while (*i < len && is_whitespace(string[*i]))
			(*i)++;
		temp = create_arg(args);
		if (!temp)
			return (NULL);
		parse_aux(args, temp, string, i);
	}
	free(i);
	return (args);
}

/* int	main(int ac, char **av)
{
	char *line = readline(">");
	(void)ac;
	char **res3;
	char **res31;
	t_arg **args;
	t_arg *temp;

	args = parse_arguments(line);
	temp = (*args);

	//printf("%s", line);

	while (args && temp)
	{
		printf("%s\n", temp->name);
		temp = temp->next;
	}
	ft_argclear(args);
}   */