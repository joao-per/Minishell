/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/05/14 12:38:32 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/14 12:38:32 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* void	cleanup(t_clean_vars *cleaning)
{
	if(cleaning->shell)
	{
		free_env(cleaning->shell);
		cleaning->shell = NULL;
	}
	if(cleaning->env_vars)
	{
		free_env_vars(cleaning->env_vars);
		cleaning->env_vars = NULL;
	}
	if(cleaning->envs)
	{
		free_envs(cleaning->envs);
		cleaning->envs = NULL;
	}
} */

void	restore_prompt(int sig)
{
	if (sig == SIGINT || (sig == SIGQUIT && g_check_exit[1] != 0))
	{
		if (sig == SIGINT)
			write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (g_check_exit[1] == 0)
			rl_redisplay();
		if (sig == SIGQUIT)
		{
			printf("Quit (core dumped)\n");
			g_check_exit[0] = 131;
		}
		else
			g_check_exit[0] = 130;
		g_check_exit[1] = 0;
	}
	else
		signal(SIGQUIT, SIG_IGN);
}
