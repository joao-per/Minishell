/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/18 14:10:39 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_new_var_format(char *new_var)
{
	if ((!ft_isalpha(new_var[0]) && !(new_var[0] == '_')))
	{
		printf("export: `%s': not a valid identifier \n", new_var);
		g_check_exit[0] = 1;
		return (0);
	}
	return (1);
}

void	update_existing_var(char *env_value, t_env *curr, char *eq_pos)
{
	free(curr->env_value);
	if (eq_pos)
		curr->env_value = ft_strdup(env_value);
	else
		curr->env_value = NULL;
}

void	create_new_var(char *new_var, char *eq_pos, t_env **env_vars)
{
	t_env	*curr;

	curr = env_new(new_var);
	env_add_back(env_vars, curr);
	if (!eq_pos)
		curr->env_value = NULL;
}

void	update_or_create_variable(t_env **env_vars, char *new_var,
		char *env_name)
{
	t_env	*curr;
	char	*eq_pos;

	eq_pos = ft_strchr(new_var, '=');
	curr = search_env_name(env_vars, env_name);
	free(env_name);
	if (curr != NULL)
		update_existing_var(eq_pos + 1, curr, eq_pos);
	else
		create_new_var(new_var, eq_pos, env_vars);
}

int	export_variable(t_env **env_vars, char *new_var)
{
	char	*env_name;
	char	*env_value;
	int		flag;

	flag = check_new_var_format(new_var);
	if (!flag)
		return (1);
	get_env_name_and_value(&env_name, &env_value, new_var);
	update_or_create_variable(env_vars, new_var, env_name);
	return (0);
}
