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

#include "minishell.h"

void	restore_prompt(int sig)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)sig;
}

void	back_slash(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
	exit(0);
}

void	setup_signals(void)
{
	struct sigaction action;

	action.sa_handler = restore_prompt;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	if (sigaction(SIGINT, &action, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}

	action.sa_handler = back_slash;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	if (sigaction(SIGQUIT, &action, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
}
