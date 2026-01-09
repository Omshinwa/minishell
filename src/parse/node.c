/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 18:14:55 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

// Create a new node in the AST
t_node	*new_node(t_node_type type, t_node *left, t_node *right)
{
	t_node	*node;

	node = safe_malloc(sizeof(t_node));
	if (!node)
		return (clean_nodes(&left), clean_nodes(&right), NULL);
	node->type = type;
	node->argv = NULL;
	node->redir = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

void	clean_nodes(t_node **node_addr)
{
	int		i;
	t_redir	*redir;
	t_redir	*redir_next;
	t_node	*node;

	if (!node_addr || !*node_addr)
		return ;
	node = *node_addr;
	clean_nodes(&node->left);
	clean_nodes(&node->right);
	i = 0;
	while (node->argv && node->argv[i])
		free(node->argv[i++]);
	free(node->argv);
	redir = node->redir;
	while (redir)
	{
		free(redir->filename);
		redir_next = redir->next;
		free(redir);
		redir = redir_next;
	}
	free(node);
	*node_addr = NULL;
}

// When you call it, give DEPTH the value 0
// This will print NODE and every children node
void	print_nodes(t_node *node, int depth)
{
	char *color;
	int	i;
	char	offset[depth * 7 + 1];
	t_redir *tmp_redir;

	if (!node)
		return ;
	i = 0;
	while (i < depth * 7)
	{
		offset[i] = ' ';
		i++;
	}
	offset[i] = '\0';
	i = 1;
	color = BG_WHITE BOLD;
	if (node->type == NODE_CMD)
	{
		i = 0;
		while (node->argv[i] != NULL && i < 5)
		{
			if (i == 4 && node->argv[5])
				printf("%s%s %13s" RESET "\n", offset, color, "...");
			else if (i == 0)
				printf("%s%s*%13s" RESET "\n", offset, color, node->argv[i]);
			else
				printf("%s%s%14s" RESET "\n", offset, color, node->argv[i]);
			i++;
		}
		tmp_redir = node->redir;
		while (tmp_redir != NULL)
		{
			color = BG_MAGENTA;
			if (tmp_redir->type == TK_REDIR_IN)
				printf("%s%s<%13s" RESET "\n", offset, color,
					tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_OUT)
				printf("%s%s>%13s" RESET "\n", offset, color,
					tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_2IN)
				printf("%s%s<<%12s" RESET "\n", offset, color,
					tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_2OUT)
				printf("%s%s>>%12s" RESET "\n", offset, color,
					tmp_redir->filename);
			tmp_redir = tmp_redir->next;
			color = BG_WHITE BOLD;
			i++;
		}
	}
	else if (node->type == NODE_PIPE)
	{
		color = BG_RED;
		printf("%s%s*PIPE: %7s" RESET "\n", offset, color, "|");
	}
	else if (node->type == NODE_AND)
	{
		color = BG_GREEN;
		printf("%s%s*AND: %8s" RESET "\n", offset, color, "&&");
	}
	else if (node->type == NODE_OR)
	{
		color = BG_YELLOW;
		printf("%s%s*OR: %9s" RESET "\n", offset, color, "||");
	}
	else if (node->type == NODE_SUBSHELL)
	{
		color = BG_BLUE;
		printf("%s%s*%13s" RESET "\n", offset, color, "SUBSHELL     ");
	}
	while (i++ < 5)
		printf("%s%s %13s" RESET "\n", offset, color, " ");
	if (node->left)
		print_nodes(node->left, depth + 1);
	if (node->right)
		print_nodes(node->right, depth + 1);
}