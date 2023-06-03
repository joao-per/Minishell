/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/06/01 23:58:45 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/01 23:58:45 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	g_exit_status;

void	init_program(char **env, t_env ***env_vars, char ***envs)
{
	*env_vars = env_init(env);
	*envs = create_env_arr(*env_vars);
}

int	main(int ac, char **argv, char **env)
{
	t_env	**env_vars;
	char	**envs;

	(void)ac;
	(void)argv;
	(void)env;
	setup_signals();
	init_program(env, &env_vars, &envs);
	main_loop(env_vars, envs);
	rl_clear_history();
	return (0);
}
