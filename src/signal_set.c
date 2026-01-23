/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 08:53:06 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_ctrl_c(int signum)
{
	(void)signum;
	mark_sigint_received();
	rl_done = 1;
}

static int	event(void)
{
	return (0);
}

void	set_signals_interactive(void)
{
	signal(SIGINT, heredoc_ctrl_c);
	rl_event_hook = event;
	signal(SIGQUIT, SIG_IGN);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
