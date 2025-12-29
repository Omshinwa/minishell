/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2025/12/15 17:33:39 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_ackowledge = 0;
// an atomic operation guarantees that every read/write from g_acknoledge are
// done in full before handling a signal.
// volatile prevents the compiler from making some optimizations

static void	acknowledge_sig(int signum)
{
	(void)signum;
	g_ackowledge = 1;
}

static int	send_char(int pid, char letter)
{
	unsigned int	i;

	i = 0;
	while (i < sizeof(char) * 8)
	{
		g_ackowledge = 0;
		if (letter & 1)
		{
			if (kill(pid, SIGUSR2) == -1)
				return (-1);
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
				return (-1);
		}
		i++;
		letter = letter >> 1;
		while (!g_ackowledge)
			pause();
	}
	return (0);
}

static int	send_string(int pid, char *string)
{
	int	c;

	while (*string)
	{
		c = *string;
		if (send_char(pid, c) == -1)
			return (-1);
		string++;
	}
	if (send_char(pid, '\0') == -1)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	int		server_pid;
	char	*string_to_print;

	ft_printf(GREEN_COLOR "Client Processor ID: %i\n" RESET_COLOR, getpid());
	signal(SIGUSR1, acknowledge_sig);
	if (argc != 3)
	{
		ft_putstr_fd(RED_COLOR "Argument Error: \n", 2);
		ft_putstr_fd("	First arg should be a Processor ID. \n", 2);
		ft_putstr_fd("	Second should be a string. \n" RESET_COLOR, 2);
		return (1);
	}
	server_pid = ft_atoi(argv[1]);
	string_to_print = argv[2];
	if (server_pid <= 0 || send_string(server_pid, string_to_print) == -1)
	{
		ft_putstr_fd(RED_COLOR "Signal Error: \n", 2);
		ft_putstr_fd("Check if the Processor ID is correct? \n" RESET_COLOR, 2);
		return (1);
	}
	ft_putstr_fd(GREEN_COLOR "Sent message.\n" RESET_COLOR, 1);
	return (0);
}
