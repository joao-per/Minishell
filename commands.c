/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/26 01:49:06 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void cd_command(char **av, t_env **env_vars)
{
	char	cwd[MAX_LINE];
	char	*oldwd;
	t_env 	*temp;

    if (av[1] == NULL)
	{
		chdir(getenv("HOME"));
	}
	else
	{
		if (chdir(av[1]) != 0)
		{
			perror("minishell");
			return;
		}
	}
	getcwd(cwd, sizeof(cwd));
	temp = search_env_name(env_vars, "PWD");
	oldwd = ft_strdup(temp->env_value);
	free(temp->env_value);
	temp->env_value = ft_strdup(cwd);
	temp = search_env_name(env_vars, "OLDPWD");
	free(temp->env_value);
	temp->env_value = ft_strdup(oldwd);
	free(oldwd);
}

int	check_commands2(t_shell *shell, t_env **child_env_vars)
{
	int i;

	if (ft_strcmp(shell->args_str[0], "cd") == 0)
	{
		cd_command(shell->args_str, child_env_vars);
		return 0;
	}
	if (ft_strcmp(shell->args_str[0], "export") == 0)
	{
		i = 1;
		if (!shell->args_str[i])
			export_variable(child_env_vars, shell->args_str[i]);
		while (shell->args_str[i])
		{
			export_variable(child_env_vars, shell->args_str[i]);
			i++;
		}
		return (0);
	}
	if (ft_strcmp(shell->args_str[0], "unset") == 0)
	{
		i = 1;
		while (shell->args_str[i])
		{
			unset_variable(child_env_vars, shell->args_str[i]);
			i++;
		}
		return (0);
	}
	//execute_external_command(av, child_env_vars);
	return (1);
}

int	check_commands(t_shell *shell, t_env **child_env_vars)
{
	char	cwd[MAX_LINE];

	if (ft_strcmp(shell->args_str[0], "pwd") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("minishell");
		else
		{
			write(STDOUT_FILENO, cwd, ft_strlen(cwd));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (0);
	}
	if (ft_strcmp(shell->args_str[0], "echo") == 0)
	{
		echo_command(shell->args_str);
		return (0);
	}
	return (check_commands2(shell, child_env_vars));
}

void	export_variable(t_env **env_vars, char *new_var)
{
	t_env	*curr;
	char *eq_pos;
	char *env_name;
	char *env_value;
	int env_name_l;

	// If no argument is provided, display all environment variables
	if (!new_var)
	{
		print_export_vars(env_vars);
		return ;
	}
	if ((!ft_isalpha(new_var[0]) && !(new_var[0] == '_')))
	{
		printf("export: `%s': not a valid identifier \n", new_var);
		return ;
	}
	eq_pos = ft_strchr(new_var, '=');
	if (eq_pos)
		env_name_l = eq_pos - new_var;
	else
		env_name_l = ft_strlen(new_var);
	env_name = malloc(env_name_l + 1);
	ft_strncpy(env_name, new_var, env_name_l);
	env_name[env_name_l] = '\0';
	env_value = eq_pos + 1;
	curr = search_env_name(env_vars, env_name);
	free(env_name);
	if(curr != NULL)
	{
		free(curr->env_value);
		if (eq_pos)
			curr->env_value = ft_strdup(env_value);
		else
			curr->env_value = NULL;
	}
	else
	{
		curr = env_new(new_var);
		env_add_back(env_vars, curr);
		if(!eq_pos)
			curr->env_value = NULL;
	}
}

void	unset_variable(t_env **env_vars, const char *var_name)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env_vars;
	prev = NULL;
	if (!var_name)
	{
		printf("minishell: unset: Invalid format. Usage: unset KEY\n");
		return ;
	}
	while (curr)
	{
		if (ft_strncmp(curr->env_name, (char *)var_name, ft_strlen(var_name)) == 0)
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
