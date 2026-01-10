/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/10 17:54:29 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"

//   █████████  █████   █████████  ██████   █████   █████████   █████      
//  ███▒▒▒▒▒███▒▒███   ███▒▒▒▒▒███▒▒██████ ▒▒███   ███▒▒▒▒▒███ ▒▒███       
// ▒███    ▒▒▒  ▒███  ███     ▒▒▒  ▒███▒███ ▒███  ▒███    ▒███  ▒███       
// ▒▒█████████  ▒███ ▒███          ▒███▒▒███▒███  ▒███████████  ▒███       
//  ▒▒▒▒▒▒▒▒███ ▒███ ▒███    █████ ▒███ ▒▒██████  ▒███▒▒▒▒▒███  ▒███       
//  ███    ▒███ ▒███ ▒▒███  ▒▒███  ▒███  ▒▒█████  ▒███    ▒███  ▒███      █
// ▒▒█████████  █████ ▒▒█████████  █████  ▒▒█████ █████   █████ ███████████
//  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒    ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒▒ 

// This is used as signal handler, when called during a readline(), it just
// reprints the prompt on a new line.
void	redisplay_prompt(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// useful
void	set_signals_interactive_mode(void)
{
	signal(SIGINT, redisplay_prompt);
	signal(SIGQUIT, SIG_IGN);
}

// useful
void	set_signals_multilines(void)
{
	signal(SIGINT, SIG_DFL);
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
