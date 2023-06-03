/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 17:06:38 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 18:40:36 by pedperei         ###   ########.fr       */
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