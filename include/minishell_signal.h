/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/13 09:52:14 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>

void	set_signals_interactive(void);
void	ignore_signals(void);
void	reset_signals(void);

int		is_sigint_received(void);
void	mark_sigint_received(void);
void	clear_sigint_received(void);

#endif