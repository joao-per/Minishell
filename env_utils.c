/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:17:23 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/24 19:04:25 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_envsize(t_env *env)
{
	t_env		*temp;
	int			i;

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

char	*ft_strncpy(char *s1, char *s2, int n)
{
	int	i;

	i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

void sep_name_value_env(t_env *new, char *env_var)
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
	if(eq_pos != 0)
		new->env_value = ft_strdup(env_value);
	else
		new->env_value = NULL;
	new->next = NULL;
}

t_env	*env_new(char *env_var)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	sep_name_value_env(new, env_var);
	return (new);
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

void	print_env_vars(t_env **env_arr)
{
	t_env	*current;

	current = *env_arr;
	while (current)
	{
		if(current->env_value)
			printf("%s=%s\n", current->env_name, current->env_value);
		current = current->next;
	}
}

void	print_export_vars(t_env **env_arr)
{
	t_env	*current;

	current = *env_arr;
	while (current)
	{
		if(current->env_value)
			printf("declare -x %s=\"%s\"\n", current->env_name, current->env_value);
		else
			printf("declare -x %s\n", current->env_name);
		current = current->next;
	}
}

t_env	*search_env_name(t_env **stack, char *var_to_find)
{
	t_env	*temp;
	int		len;

	temp = (*stack);
	while (temp != NULL)
	{
		if (!ft_strcmp(temp->env_name, var_to_find))
			break ;
		temp = temp->next;
	}
	return (temp);
}

char *get_env_value(char *name, t_env **env_vars)
{
	t_env *env_var;
	char *value;
	int name_len;

	name_len = strlen(name);
	env_var = *env_vars;
	while (env_var)
	{
		if (ft_strncmp(env_var->env_name, name, name_len) == 0)
		{
			value = strdup(env_var->env_value);
			return value;
		}
		env_var = env_var->next;
	}
	return (NULL);
}