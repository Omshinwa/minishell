/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/15 06:54:23 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (!str || !ft_strlen(str))
		return ;
	add_history(str);
	shell->node = get_abstract_syntax_tree(str, shell);
	if (shell->node)
	{
		exit_status = execute_tree(shell->node, shell);
		shell->last_exit = exit_status;
		clean_nodes(&shell->node);
	}
}

static void	update_sigint_receiver(t_shell *shell)
{
	if (is_sigint_received())
	{
		shell->last_exit = 130;
		clear_sigint_received();
	}
}

// if (isatty(fileno(stdin)))
// 	shell->prompt = readline(shell->terminal_prompt);
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
		line = readline("minishell> ");
		update_sigint_receiver(shell);
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		parse_and_run_command(line, shell);
		free(line);
	}
	return (exit_code = shell->last_exit, clean_shell(shell), exit_code);
}
