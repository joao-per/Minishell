/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/06/03 12:30:42 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 12:30:42 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_var(t_env **env_vars, const char *var_name)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_vars;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->env_name, (char *)var_name,
				ft_strlen(var_name)) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_vars = curr->next;
			free(curr->env_name);
			free(curr->env_value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	printf("minishell: unset: Variable not found: %s\n", var_name);
}

void	validate_var_name(const char *var_name)
{
	if (!var_name)
	{
		printf("minishell: unset: Invalid format. Usage: unset KEY\n");
		exit(1);
	}
}
