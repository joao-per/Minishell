/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 00:35:45 by pedperei         ###   ########.fr       */
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