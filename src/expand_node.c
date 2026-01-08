/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 08:42:52 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/08 19:59:41 by wiwu             ###   ########.fr       */
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
	expanded = malloc(sizeof(char *) * (count + 1));
	if (!expanded)
		return (NULL);
	while (i < count)
	{
		expanded[i] = expand_variables(argv[i], shell);
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
			expanded = expand_variables(redir->filename, shell);
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
	char	**expanded_argv;

	if (node->argv)
	{
		expanded_argv = expand_argv(node->argv, shell);
		if (expanded_argv)
		{
			// free(node->argv); // j'ai passe 1h a trouver cette ligne qui fait
			// bugger mon clean_nodes, tu peux pas juste free ca, faut
			// que tu free les argv[i] dedans d'abord.
			// maybe faire un util qui clean un array de pointeur? j'ai recopie
			// mon bout de code de clean_nodes ici
			int i = 0;
			while (node->argv && node->argv[i])
				free(node->argv[i++]);
			free(node->argv);
			node->argv = expanded_argv;
		}
	}
	if (node->redir)
		expand_redirections(node->redir, shell);
}

void	expand_node(t_node *node, t_shell *shell)
{
	if (!node)
		return ;
	expand_node(node->left, shell);
	expand_node(node->right, shell);
	if (node->type == NODE_CMD)
		expand_cmd_node(node, shell);
}
