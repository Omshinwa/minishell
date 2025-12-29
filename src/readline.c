/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2025/12/29 15:25:03 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// I dont think you can just use split if you have to handle ' " escape chars
// TODO handle errors
t_node	**parse(char *str)
{
	t_node	**node_list; 
	t_node	*current_node; 
	char	**str_list;
	int		i;
	int		len_arg;

	str_list = ft_split(str, '|');
	i = 0;
	while(str_list[i] != NULL)
		i++;
	len_arg = i;
	node_list = calloc(sizeof(t_node), len_arg);
	i = -1;
	while(++i < len_arg)
	{
		current_node = calloc(sizeof(t_node), 1);
		current_node->cmd_args = ft_split(str_list[i], ' ');
		node_list[i] = current_node;
	}
	node_list[i] = NULL;
	return (node_list);
}

void print_list_of_cmds(t_node **node_list)
{
	int	i;
	int	j;
	t_node	*node;

	i = -1;
	while(node_list[++i])
	{
		node = node_list[i];
		printf(BOLD "CMD: %s" RESET, node->cmd_args[0]);
		j = 0;
		while(node->cmd_args[++j])
		{
			printf(" arg%i: %s", j, node->cmd_args[j]);
		}
		printf("\n|\n");
	}
}

volatile sig_atomic_t	g_ackowledge = 0;
// an atomic operation guarantees that every read/write from g_acknoledge are
// done in full before handling a signal.
// volatile prevents the compiler from making some optimizations

static void	acknowledge_sig(int signum)
{
	g_ackowledge = signum;
	// printf("%i received %i \n", getpid(), signum);
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
	}
}

int main(void)
{
	char *str;
	t_node	**node_list; 

	signal(SIGINT, acknowledge_sig);
	signal(SIGQUIT, SIG_IGN);
	// fork();
	while(1)
	{
		str = readline("👻>");
		if (!str)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(str))
		{
			add_history(str);
			node_list = parse(str);
			print_list_of_cmds(node_list);
		}
		free(str);
	}
	return (0);
}
