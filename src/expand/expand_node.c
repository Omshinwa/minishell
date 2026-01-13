/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 08:42:52 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/13 12:32:54 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		expanded[i] = remove_quotes(argv[i], shell);
		if (!expanded[i])
			return (free_argv(expanded),
				print_red("variable expansion failed"), NULL);
		i++;
	}
	expanded[i] = NULL;
	return (expanded);
}

static int	expand_redir_wildcard(t_redir *redir)
{
	char	**matches;

	if (!has_wildcard(redir->filename))
		return (0);
	matches = expand_single_wildcard(redir->filename);
	if (!matches || !matches[0])
		return (0);
	if (matches[1])
	{
		free_argv(matches);
		print_red("minishell: ambiguous redirect");
		return (-1);
	}
	free(redir->filename);
	redir->filename = ft_strdup(matches[0]);
	free_argv(matches);
	return (0);
}

void	expand_redirections(t_redir *redir, t_shell *shell)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type != TK_REDIR_2IN)
		{
			expanded = remove_quotes(redir->filename, shell);
			if (expanded)
			{
				free(redir->filename);
				redir->filename = expanded;
			}
			if (expand_redir_wildcard(redir) == -1)
				return ;
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

// after expanding the vars, shift_all_argv if there's an empty arg
void	expand_node(t_node *node, t_shell *shell)
{
	if (!node)
		return ;
	expand_node(node->left, shell);
	expand_node(node->right, shell);
	if (node->type == NODE_CMD)
	{
		shift_all_args(node->argv);
		expand_cmd_node(node, shell);
	}
}
