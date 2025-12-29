/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2025/12/15 18:32:00 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	add_letter_to_str(char letter, t_buff *s_buffer)
{
	int		i;
	char	*new_str;

	i = 0;
	if (s_buffer->buffer_left == 0)
	{
		s_buffer->buffer_left = 1024;
		new_str = malloc(sizeof(char) * (s_buffer->len + 1024));
		if (!new_str)
		{
			ft_printf(RED_COLOR "Error, text too long. Exiting program");
			ft_printf("Exiting program \n" RESET_COLOR);
			return (free(s_buffer->string), exit(-1), -1);
		}
		ft_memcpy(new_str, s_buffer->string, s_buffer->len * sizeof(char));
		free(s_buffer->string);
		s_buffer->string = new_str;
	}
	s_buffer->string[s_buffer->len] = letter;
	s_buffer->len++;
	s_buffer->buffer_left--;
	return (0);
}

static void	signal_handler(int signum, siginfo_t *info, void *ucontext)
{
	static int		counter = 0;
	static int		binary_repr = 0;
	static t_buff	s_buffer = (t_buff){NULL, 0, 0};

	(void)ucontext;
	if (signum == SIGUSR2)
		binary_repr |= (1 << counter);
	counter++;
	if (counter == 8)
	{
		if (binary_repr == '\0')
		{
			write(1, s_buffer.string, s_buffer.len);
			free(s_buffer.string);
			s_buffer = (t_buff){NULL, 0, 0};
		}
		else
			add_letter_to_str((char)binary_repr, &s_buffer);
		binary_repr = 0;
		counter = 0;
	}
	kill(info->si_pid, SIGUSR1);
}
// binary_repr |= (1 << counter) : this is equivalent to
// adding 2**counter
// last line is the ACK
// siginfo_t has the si_pid field which holds the Process ID.

// 	struct sigaction {
// 	void     (*sa_handler)(int);
// 	void     (*sa_sigaction)(int, siginfo_t *, void *);
// 	sigset_t   sa_mask;
// 	int        sa_flags;
// 	void     (*sa_restorer)(void);
// };

static int	set_up_actions(void)
{
	struct sigaction	action;

	action.sa_sigaction = signal_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGUSR1);
	sigaddset(&action.sa_mask, SIGUSR2);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	return (0);
}

int	main(void)
{
	int	pid;

	set_up_actions();
	pid = getpid();
	ft_printf(GREEN_COLOR "Server Processor ID: %i\n" RESET_COLOR, pid);
	while (1)
		pause();
	return (0);
}
