/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/10 19:58:14 by wiwu             ###   ########.fr       */
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
	shell->node = NULL;
	return (shell);
}

// If there's an issue and you need to debug add the print:
// expand_node(shell->node, shell);
// print_nodes(shell->node, 0);
static void	parse_and_run_command(char *str, t_shell *shell)
{
	int		exit_status;

	exit_status = shell->last_exit;
	if (!str || !ft_strlen(str))
		return ;
	add_history(str);
	shell->node = get_abstract_syntax_tree(str, shell);
	if (shell->node)
	{
		expand_node(shell->node, shell);
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
