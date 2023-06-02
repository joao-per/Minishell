/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 23:58:45 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/01 23:58:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Libft/libft.h"
#include "minishell.h"

int exit_status;

void init_program(char **env, t_env ***env_vars, char ***envs)
{
	*env_vars = env_init(env);
	*envs = create_env_arr(*env_vars);
}


t_shell *init_shell(t_env **env_vars, t_arg **args, char **envs)
{
    t_shell *shell;
    shell = shell_init(env_vars, args, envs, create_args_arr(args));
    return shell;
}

void	cleanup_after_command(t_shell *shell)
{
	free_args(shell, shell->len_args);
	free(shell);
}

t_arg **process_input(char **line, t_env **env_vars) {
    char *line_exp;

    *line = readline("\033[0;93m Minishell>$ \033[0;39m");
    if (*line == NULL)
        return NULL;

    line_exp = treat_expansion(*line, env_vars);
    add_history(*line);
    free(*line);
    return parse_arguments(line_exp);
}

int check_command(t_arg **args, t_env **env_vars) {
    if (!args || (*args)->name == NULL) {
        return 0;
    } else if (ft_strcmp((*args)->name, "exit") == 0) {
        return -1;
    } else if (ft_strcmp((*args)->name, "env") == 0) {
        print_env_vars(env_vars);
        return 0;
    } 
    return 1;
}

void main_loop(t_env **env_vars, char **envs)
{
    int should_run;
    t_shell *shell;
    t_arg **args;
    char *line;

	shell = NULL;
    should_run = 1;
    while (should_run)
	{
        setup_signals();
        args = process_input(&line, env_vars);
        if (!args)
		{
            //free(line);
			if (!line)
                break;
            continue;
        }
        shell = init_shell(env_vars, args, envs);
        should_run = check_command(args, env_vars); 
        if (should_run == 1)
		{
            execute_command(shell, env_vars);
            cleanup_after_command(shell);
        }
		else if (should_run == -1)
            break;
    }
	if(shell)
    	free_env(shell);
}

int	main(int ac, char **argv, char **env) {
    t_env **env_vars;
    char **envs;

    (void)ac;
    (void)argv;
    (void)env;

    init_program(env, &env_vars, &envs);
    main_loop(env_vars, envs);

    return (0);
}