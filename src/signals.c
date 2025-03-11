/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:47:44 by sruzzi            #+#    #+#             */
/*   Updated: 2025/03/07 15:48:04 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	handle_signals(int signo)
{
	if (g_child_pid != 0) // S'il y a un processus en cours (comme `cat` ou `sleep`)
	{
		kill(g_child_pid, signo); // Envoyer le signal au fils
		return ;
	}
	if (signo == SIGINT) // Ctrl+C
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}

}

void	setup_signals(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, SIG_IGN); // On ignore SIGQUIT dans le shell parent
}
