/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 08:52:26 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Malloc a new REDIR
// sets the fields correctly (and move up the ctx)
// add it to the end of NODE->REDIR
static t_redir	*create_n_append_new_redir(t_node *node, t_ctx_parser *ctx)
{
	t_redir	*redir;
	t_redir	*last_redir;

	redir = safe_malloc(sizeof(t_redir));
	if (!redir)
		return (ctx->i++, NULL);
	redir->type = ctx->arr_token[ctx->i].type;
	ctx->i++;
	redir->filename = ft_strdup(ctx->arr_token[ctx->i].str);
	if (!redir->filename)
	{
		free(redir);
		return (ctx->i++, NULL);
	}
	redir->next = NULL;
	redir->fd = -1;
	ctx->i++;
	last_redir = node->redir;
	if (!last_redir)
		return (node->redir = redir, redir);
	while (last_redir->next)
		last_redir = last_redir->next;
	last_redir->next = redir;
	return (redir);
}

// This is called when the token at ARR_TOKEN[*INDEX] is < or > or << or >>:
// calls CREATE N APPEND NEW REDIR
// if TK_REDIR_2IN : Create a fork child that calls heredoc
static int	parse_redirection(t_node *node,	t_ctx_parser *ctx)
{
	t_redir	*redir;
	int		fd[2];
	int		status;

	redir = create_n_append_new_redir(node, ctx);
	if (!redir)
		return (-1);
	if (redir->type == TK_REDIR_2IN)
	{
		pipe(fd);
		if (fork_heredoc_child(node, ctx, redir->filename, fd))
			return (close(fd[0]), close(fd[1]), -1);
		ignore_signals();
		wait(&status);
		close(fd[1]);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				mark_sigint_received();
			close(fd[0]);
			return (write(STDOUT_FILENO, "\n", 1), rl_on_new_line(), -1);
		}
		redir->fd = fd[0];
	}
	return (0);
}

// Check if there's anything wrong with the command (redirection 
// without filename, parenthesis in the middle)
// return the malloc size for argv.
static int	get_argv_size(t_ctx_parser *ctx)
{
	int		nb_of_words;
	int		i;

	i = ctx->i;
	nb_of_words = 0;
	while (ctx->arr_token[i].type & (TK_WORD
			| TK_REDIR_IN | TK_REDIR_OUT | TK_REDIR_2IN | TK_REDIR_2OUT))
	{
		if (ctx->arr_token[i].type == TK_WORD)
			nb_of_words++;
		else
		{
			if (ctx->arr_token[++i].type != TK_WORD)
			{
				print_red("redirection expected a filename.");
				if (ctx->arr_token[i].str)
					print_2red("got instead ", ctx->arr_token[i].str);
				else
					print_red("got instead (nil)");
				return (-1);
			}
		}
		i++;
	}
	return (nb_of_words);
}

static int	populate_argv(t_ctx_parser *ctx, t_node *node)
{
	int		i;

	i = 0;
	while (ctx->arr_token[ctx->i].type & (TK_WORD | TK_REDIR_IN | TK_REDIR_OUT
			| TK_REDIR_2IN | TK_REDIR_2OUT))
	{
		if (ctx->arr_token[ctx->i].type == TK_WORD)
		{
			node->argv[i++] = ft_strdup(ctx->arr_token[ctx->i].str);
			ctx->i++;
		}
		else
		{
			if (parse_redirection(node, ctx) == -1)
				return (node->argv[i] = NULL, -1);
		}
	}
	node->argv[i] = NULL;
	return (0);
}

// Consumes WORDS and REDIR tokens.
// a SIMPLE COMMAND, is made of REDIRECTIONS and WORDS, no other operators.
// CTX should always be pointing at a WORD / REDIR token.
// Should never have an &&, ||, pipe or () token OR END token.
t_node	*parse_simple_command(t_ctx_parser *ctx)
{
	t_node	*node;
	int		nb_of_words;

	if (!(ctx->arr_token[ctx->i].type & (TK_WORD | TK_REDIR_IN
				| TK_REDIR_OUT | TK_REDIR_2IN | TK_REDIR_2OUT)))
		return (print_2red("unexpected token", ctx->arr_token[ctx->i].str),
			NULL);
	nb_of_words = get_argv_size(ctx);
	if (nb_of_words == -1)
		return (NULL);
	node = new_node(NODE_CMD, NULL, NULL);
	node->argv = safe_malloc((nb_of_words + 1) * sizeof(char *));
	if (!node->argv)
		return (clean_nodes(&node), NULL);
	if (populate_argv(ctx, node) == -1)
		return (clean_nodes(&node), NULL);
	return (node);
}
