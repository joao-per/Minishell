/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 12:38:32 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/14 12:38:32 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void restore_prompt(int sig, siginfo_t *info, void *ucontext)
{
    int *ret_number = info->si_value.sival_ptr;

    *ret_number = 130;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
    (void)sig;
    (void)ucontext;
}

void ctrl_c(int sig)
{
    write(1, "\n", 1);
    (void)sig;
}

void back_slash(int sig)
{
    printf("Quit (core dumped)\n");
    (void)sig;
}

// Signal setup function

void run_signals(int sig, int *ret_number)
{
    struct sigaction action;

    if (sig == 1)
    {
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = restore_prompt;
        sigaction(SIGINT, &action, NULL);
        action.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &action, NULL);
        action.sa_sigaction = restore_prompt;
        action.sa_flags = SA_SIGINFO;
        sigemptyset(&action.sa_mask);
        action.sa_restorer = NULL;
        action.sa_flags = 0;
        sigaction(SIGINT, &action, NULL);
    }
    else if (sig == 2)
    {
        action.sa_handler = ctrl_c;
        sigaction(SIGINT, &action, NULL);
        action.sa_handler = back_slash;
        sigaction(SIGQUIT, &action, NULL);
    }
    else if (sig == 3)
    {
        printf("exit\n");
        exit(0);
    }
    action.sa_sigaction = NULL;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_restorer = NULL;
}