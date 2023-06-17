/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 19:56:47 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/17 19:57:10 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*search_env_name(t_env **stack, char *var_to_find)
{
	t_env	*temp;

	temp = (*stack);
	while (temp != NULL)
	{
		if (!ft_strcmp(temp->env_name, var_to_find))
			break ;
		temp = temp->next;
	}
	return (temp);
}

char	*get_env_value(char *name, t_env **env_vars)
{
	t_env	*env_var;
	char	*value;
	int		name_len;

	name_len = strlen(name);
	env_var = *env_vars;
	while (env_var)
	{
		if (ft_strncmp(env_var->env_name, name, name_len) == 0)
		{
			value = strdup(env_var->env_value);
			return (value);
		}
		env_var = env_var->next;
	}
	return (NULL);
}

t_env	**env_init(char **env)
{
	int		i;
	t_env	**env_arr;
	t_env	*env_var;

	i = 0;
	env_arr = (t_env **)ft_calloc(1, sizeof(t_env *));
	while (env[i])
	{
		env_var = env_new(env[i]);
		env_add_back(env_arr, env_var);
		i++;
	}
	return (env_arr);
}
