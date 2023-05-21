/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 20:16:42 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/21 18:57:41 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

char *expansion(char *input, char *str_exp, t_env **env, int *i)
{
	int start;
	int end;
	char *search;
	t_env *env_var;

	start = *i;
	(*i)++;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	end = *i;
	search = ft_substr(input, start, end - start);
	env_var = search_env_name(env, search);
	free(search);
	end = ft_strlen(input) - (end - start + 1) + ft_strlen(env_var->env_name);
	search = ft_calloc(end + 1, sizeof(char));
	start = -1;
	while (str_exp[++start] != '$')
		search[start] = str_exp[start];
	end = -1;
	while (env_var->env_name[++end])
	{
		search[start] = env_var->env_name[end];
		start++;
	}
	free(str_exp);
	return (search);
}

void	exp_quote_update(char c, int *flag_type, int *quotes_open)
{
	if (quote_type(c))
	{
		if (*quotes_open == 0)
		{
			*flag_type = quote_type(c);
			*quotes_open = 1;
		}
		else if (*quotes_open == 1)
			if (*flag_type == quote_type(c))
			{
				*quotes_open = 0;
				*flag_type = 0;
			}
	}
}

char	*treat_expansion(char *input, t_env **env)
{
	int		*i;
	int		*quotes_open;
	int		*flag_type;
	char	*str_exp;

	quotes_open = 0;
	flag_type = 0;
	str_exp = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	i = ft_calloc(1, sizeof(int));
	while (input[*i])
	{
		exp_quote_update(input[*i], flag_type, quotes_open);
		if (input[*i] == '$' && *flag_type != 1)
			str_exp = expansion(input, str_exp, env, i);
		else
		{
			str_exp[*i] = input[*i];
			(*i)++;
		}
	}
	return (str_exp);
}

/* int	main(int ac, char **av, char **env)
{
	char *line = readline(">");
	(void)ac;
	char *res3;
	char **res31;
	t_arg **args;
	t_env **env_vars;
	t_arg *temp;

	env_vars = env_init(env);
	res3 = treat_expansion(line, env_vars);

	printf("%s", res3);

	
}  
 */