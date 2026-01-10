/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/10 05:54:17 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"
#include "minishell_signal.h"
#include "minishell.h"

// Append REDIR to the last NODE->next
static void	redir_append(t_node *node, t_redir *redir)
{
	t_redir	*tmp;

	tmp = node->redir;
	if (!tmp)
	{
		node->redir = redir;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
}

// When the token at ARR_TOKEN[*INDEX] is < or > or << or >>:
// Calls read_heredoc_fd
// Sets / append the NODE->REDIR
static int	parse_redirection(t_node *node,	t_ctx_parser *ctx)
{
	t_redir	*redir;

	redir = safe_malloc(sizeof(t_redir));
	if (!redir)
		return (ctx->i++, -1);
	redir_append(node, redir);
	redir->type = ctx->arr_token[ctx->i].type;
	ctx->i++;
	redir->filename = ft_strdup(ctx->arr_token[ctx->i].str);
	redir->next = NULL;
	redir->fd = -1;
	ctx->i++;
	if (redir->type == TK_REDIR_2IN)
	{
		int fd[2];
		int pid;
		int	status;
	
		status = 0;
		pipe(fd);
		pid = fork();
		if (pid == 0) // child
		{
			close(fd[0]);
			set_signals_multilines();
			read_heredoc_fd(fd[1], redir->filename);
			close(fd[1]);
			clean_tokens(&ctx->arr_token);
			clean_shell(ctx->shell);
			clean_nodes(&node);
			exit(0);
		}
		else if (pid == -1) // child
			return(print_red("pid"), -1);
		else
		{
			close(fd[1]);
			ignore_signals();
			wait(&status);
		}
		if (WIFSIGNALED(status)) // KILLED BY CTRL + C
		{
			close(fd[0]);
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			return (-1);
		}
		redir->fd = fd[0];
	}
	return (0);
}

// Check if there's anything wrong with the command (redirection 
// without filename, parenthesis in the middle)
// return the malloc size for argv.
int	get_argv_size(t_ctx_parser *ctx)
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
			i++;
			if (ctx->arr_token[i].type != TK_WORD)
			{
				print_red("redirection expected a filename.");
				printf(RED"got `%s` instead."RESET"\n", ctx->arr_token[i].str);
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
		return (printf(RED"unexpected token `%s`, expected command"RESET"\n",
				ctx->arr_token[ctx->i].str), NULL);
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
