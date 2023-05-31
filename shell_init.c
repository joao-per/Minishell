/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:12:13 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/31 22:06:22 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

t_arg	*get_arg_byindex(t_shell *shell, int index)
{
	t_arg	*temp;
	int		i;

	temp = (*shell->args);
	i = 0;
	while (temp != NULL)
	{
		if (i == index)
			return (temp);
		temp = temp->next;
		i++;
	}
	return (NULL);
}

char	**create_env_arr(t_env **envs)
{
	char	**env;
	char	*temp_str;
	t_env	*temp;
	int		i;

	temp = (*envs);
	i = 0;
	env = (char **)ft_calloc(ft_envsize(*envs) + 1, sizeof(char *));
	while (temp)
	{
		temp_str = ft_charjoin(temp->env_name, '=');
		env[i] = ft_strjoin(temp_str, temp->env_value);
		free(temp_str);
		temp = temp->next;
		i++;
	}
	env[i] = 0;
	return (env);
}

char	**create_args_arr(t_arg **args)
{
	char	**av;
	t_arg	*temp;
	int		i;

	temp = (*args);
	i = 0;
	if (!is_pipe_reds_valid(args))
	{
		free_args_struct(args);
		return (0);
	}
	av = (char **)ft_calloc(ft_argsize(*args) + 1, sizeof(char *));
	while (temp)
	{
		av[i] = ft_strdup(temp->name);
		temp = temp->next;
		i++;
	}
	av[i] = 0;
	return (av);
}

t_shell	*shell_init(t_env **env_vars, t_arg **args, char **envs, char **av)
{
	t_shell	*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (0);
	shell->envs = env_vars;
	shell->args = args;
	shell->envs_str = envs;
	shell->args_str = av;
	shell->len_args = count_strings(av);
	return (shell);
}
