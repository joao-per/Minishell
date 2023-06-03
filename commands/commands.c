/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 15:34:16 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_command(t_shell *shell)
{
	if (ft_strcmp(shell->args_str[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "echo") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "cd") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "export") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "unset") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "env") == 0)
		return (1);
	return (0);
}

int	check_commands(t_shell *shell, pid_t pid)
{
	if (pid != 0)
	{
		if (!handle_pwd(shell))
			return (0);
		if (!handle_echo(shell))
			return (0);
		if (!handle_env(shell))
			return (0);
		if (!handle_export(shell))
			return (0);
		if (!handle_cd(shell))
			return (0);
		if (!handle_unset(shell))
			return (0);
	}
	return (1);
}

int	check_new_var_format(char *new_var)
{
	if ((!ft_isalpha(new_var[0]) && !(new_var[0] == '_')))
	{
		printf("export: `%s': not a valid identifier \n", new_var);
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

void	get_env_name_and_value(char **env_name, char **env_value, char *new_var)
{
	char	*eq_pos;
	int		env_name_l;

	eq_pos = ft_strchr(new_var, '=');
	if (eq_pos)
		env_name_l = eq_pos - new_var;
	else
		env_name_l = ft_strlen(new_var);
	*env_name = malloc(env_name_l + 1);
	ft_strncpy(*env_name, new_var, env_name_l);
	(*env_name)[env_name_l] = '\0';
	*env_value = eq_pos + 1;
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

void	export_variable(t_env **env_vars, char *new_var)
{
	char	*env_name;
	char	*env_value;

	if (!check_new_var_format(new_var))
		return ;
	get_env_name_and_value(&env_name, &env_value, new_var);
	update_or_create_variable(env_vars, new_var, env_name);
}

void	unset_variable(t_env **env_vars, const char *var_name)
{
	validate_var_name(var_name);
	delete_var(env_vars, var_name);
}

void	echo_command(char **av)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (av[i] && ft_strcmp(av[i], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	while (av[i])
	{
		write(STDOUT_FILENO, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
}
