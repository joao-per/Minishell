/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:35:26 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/26 10:28:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_envsize(t_env *env)
{
	t_env	*temp;
	int		i;

	temp = env;
	i = 1;
	if (env == NULL)
		return (0);
	while (temp->next != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

t_env	*env_last(t_env *env)
{
	if (!env)
		return (0);
	while (env->next)
		env = env->next;
	return (env);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*last;

	if (!new || !env)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	last = env_last(*env);
	last->next = new;
}

t_env	*env_new(char *env_var)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	sep_name_value_env(new, env_var);
	return (new);
}

void	sep_name_value_env(t_env *new, char *env_var)
{
	char	*eq_pos;
	char	*env_name;
	char	*env_value;
	int		env_name_l;

	if (!new)
		return ;
	eq_pos = ft_strchr(env_var, '=');
	if (eq_pos)
		env_name_l = eq_pos - env_var;
	else
		env_name_l = ft_strlen(env_var);
	env_name = malloc(env_name_l + 1);
	ft_strncpy(env_name, env_var, env_name_l);
	env_name[env_name_l] = '\0';
	env_value = eq_pos + 1;
	new->env_name = env_name;
	if (eq_pos != 0)
		new->env_value = ft_strdup(env_value);
	else
		new->env_value = NULL;
	new->next = NULL;
}
