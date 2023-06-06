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
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	action;

	signal(SIGQUIT, SIG_IGN);
	action.sa_handler = restore_prompt;
	action.sa_flags = SA_RESTART;
	sigemptyset(&action.sa_mask);
	if (sigaction(SIGINT, &action, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
}

