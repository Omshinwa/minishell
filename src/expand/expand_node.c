/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 08:42:52 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/12 20:06:19 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

char	**expand_argv(char **argv, t_shell *shell)
{
	int		i;
	int		count;
	char	**expanded;

	if (!argv)
		return (NULL);
	i = 0;
	count = 0;
	while (argv[count])
		count++;
	expanded = ft_calloc(count + 1, sizeof(char *));
	if (!expanded)
		return (NULL);
	while (i < count)
	{
		expanded[i] = expand_strings_options(argv[i], shell, 1, 0, 0);
		if (!expanded[i])
			return (free_argv(expanded),
				print_red("variable expansion failed"), NULL);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

void	expand_redirections(t_redir *redir, t_shell *shell)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type != TK_REDIR_2IN)
		{
			expanded = expand_strings_options(redir->filename, shell, 1, 0, 0);
			if (expanded)
			{
				free(redir->filename);
				redir->filename = expanded;
			}
		}
		redir = redir->next;
	}
}

void	expand_cmd_node(t_node *node, t_shell *shell)
{
	int		i;
	char	**expanded_argv;
	char	**wildcards_argv;

	if (node->argv)
	{
		expanded_argv = expand_argv(node->argv, shell);
		if (expanded_argv)
		{
			i = 0;
			wildcards_argv = expand_wildcards_in_argv(expanded_argv);
			while (node->argv && node->argv[i])
				free(node->argv[i++]);
			free(node->argv);
			if (wildcards_argv)
			{
				free_argv(expanded_argv);
				node->argv = wildcards_argv;
			}
			else
				node->argv = expanded_argv;
		}
	}
	if (node->redir)
		expand_redirections(node->redir, shell);
}

// Sometimes the argv[0] ends up being empty after expansion, we shift all argv then:
void	shift_all_argv(t_node *node)
{
	int	i;
	int	size;

	i = -1;
	size = 0;
	while (node->argv[++i] != NULL)
		size++;
	free(node->argv[0]);
	i = -1;
	while (++i < size)
		node->argv[i] = node->argv[i + 1];
}

void	expand_node(t_node *node, t_shell *shell)
{
	if (!node)
		return ;
	expand_node(node->left, shell);
	expand_node(node->right, shell);
	if (node->type == NODE_CMD)
	{
		expand_cmd_node(node, shell);
		if (node->argv[0] != NULL && node->argv[1] != NULL && node->argv[0][0] == '\0')
			shift_all_argv(node);
	}
}