/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/05 18:55:04 by pedperei         ###   ########.fr       */
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
	temp = search_env_name(env_vars, "PWD=");
	oldwd = ft_strdup(temp->env_var);
	free(temp->env_var);
	temp->env_var = ft_strjoin("PWD=", cwd);
	temp = search_env_name(env_vars, "OLDPWD=");
	free(temp->env_var);
	temp->env_var = ft_strjoin("OLD", oldwd);
	free(oldwd);
}

int	check_commands(char **av, t_env **child_env_vars)
{
	char	cwd[MAX_LINE];
	if (strcmp(av[0], "cd") == 0)
	{
		cd_command(av, child_env_vars);
	}
	if (strcmp(av[0], "pwd") == 0)
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
	if (strcmp(av[0], "export") == 0)
	{
		export_variable(child_env_vars, av[1]);
		return (0);
	}
	if (strcmp(av[0], "unset") == 0)
	{
		unset_variable(child_env_vars, av[1]);
		return (0);
	}
	if (strcmp(av[0], "echo") == 0)
	{
		echo_command(av);
		return (0);
	}
	return (check_commands2(av, child_env_vars));
}

int	check_commands2(char **av, t_env **child_env_vars)
{
	struct dirent	*entry;
	DIR				*dir;
	char			*dir_path;

	if (strcmp(av[0], "ls") == 0)
	{
		if (av[1] == NULL)
			dir_path = ".";
		else
			dir_path = av[1];
		if ((dir = opendir(dir_path)) == NULL)
			perror("minishell");
		else
		{
			while ((entry = readdir(dir)) != NULL)
			{
				write(1, entry->d_name, ft_strlen(entry->d_name));
				write(STDOUT_FILENO, "\n", 1);
			}
			closedir(dir);
		}
		return (0);
	}
	execute_external_command(av, child_env_vars);
	return (1);
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
	if (!ft_isalpha(new_var[0]) && !(new_var[0] == '_') || !ft_strchr(new_var, '='))
	{
		printf("export: not an identifier: %s\n", new_var);
		return ;
	}
	eq_pos = ft_strchr(new_var, '=');
	env_name_l = eq_pos - new_var;
	env_name = malloc(env_name_l + 1);
	ft_strncpy(env_name, new_var, env_name_l);
	env_name[env_name_l] = '\0';
	env_value = eq_pos + 1;
	curr = search_env_name(env_vars, env_name);
	if(curr != NULL)
	{
		free(curr->env_var);
		//free(curr->env_value);
		curr->env_value = env_value;
		curr->env_var = new_var;
	}
	else
	{
		curr = env_new(new_var);
		env_add_back(env_vars, curr);
	}
}

void	unset_variable(t_env **env_vars, const char *var_name)
{
	t_env	*curr;
	t_env	*prev;
	char	*equal_pos;

	curr = *env_vars;
	prev = NULL;
	if (!var_name)
	{
		printf("minishell: unset: Invalid format. Usage: unset KEY\n");
		return ;
	}
	while (curr)
	{
		equal_pos = ft_strchr(curr->env_var, '=');
		if (equal_pos && strncmp(curr->env_var, var_name,
				equal_pos - curr->env_var) == 0
			&& ft_strlen(var_name) == (size_t)(equal_pos - curr->env_var))
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_vars = curr->next;

			free(curr->env_var);
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
	if (av[i] && strcmp(av[i], "-n") == 0)
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
