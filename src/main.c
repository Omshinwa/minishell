/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 10:30:21 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_signal.h"

//  ██████   ██████   █████████   █████ ██████   █████
// ▒▒██████ ██████   ███▒▒▒▒▒███ ▒▒███ ▒▒██████ ▒▒███ 
//  ▒███▒█████▒███  ▒███    ▒███  ▒███  ▒███▒███ ▒███ 
//  ▒███▒▒███ ▒███  ▒███████████  ▒███  ▒███▒▒███▒███ 
//  ▒███ ▒▒▒  ▒███  ▒███▒▒▒▒▒███  ▒███  ▒███ ▒▒██████ 
//  ▒███      ▒███  ▒███    ▒███  ▒███  ▒███  ▒▒█████ 
//  █████     █████ █████   █████ █████ █████  ▒▒█████
// ▒▒▒▒▒     ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒ 

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = init_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->last_exit = 0;
	shell->envp = envp;
	shell->state = STATE_INTERACTIVE;
	return (shell);
}

static void	parse_and_run_command(char *str, t_shell *shell)
{
	int		exit_status;

	exit_status = shell->last_exit;
	if (!str || !ft_strlen(str))
		return ;
	add_history(str);
	shell->node = get_abstract_syntax_tree(str);
	if (shell->node)
	{
		expand_node(shell->node, shell);
		print_nodes(shell->node, 0);
		exit_status = execute_tree(shell->node, shell);
		shell->last_exit = exit_status;
		clean_nodes(&shell->node);
	}
	else
		shell->last_exit = 1;
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	*shell;
	int		exit_code;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	while (1)
	{
		set_signals_interactive_mode();
		if (shell->last_exit)
			printf("\n");
			// printf(" last exit: %i\n", shell->last_exit);
		line = readline("👻👻> ");
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		parse_and_run_command(line, shell);
		free(line);
	}
	exit_code = shell->last_exit;
	clean_shell(shell);
	return (exit_code);
}
