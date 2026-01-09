/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 18:01:30 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

//  ███████████    █████████   ███████████    █████████  ██████████
// ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███ ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███▒▒███▒▒▒▒▒█
//  ▒███    ▒███ ▒███    ▒███  ▒███    ▒███ ▒███    ▒▒▒  ▒███  █ ▒ 
//  ▒██████████  ▒███████████  ▒██████████  ▒▒█████████  ▒██████   
//  ▒███▒▒▒▒▒▒   ▒███▒▒▒▒▒███  ▒███▒▒▒▒▒███  ▒▒▒▒▒▒▒▒███ ▒███▒▒█   
//  ▒███         ▒███    ▒███  ▒███    ▒███  ███    ▒███ ▒███ ▒   █
//  █████        █████   █████ █████   █████▒▒█████████  ██████████
// ▒▒▒▒▒        ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒▒ 

//					parse_expression
//						↓
//					parse_pipeline
//						↓
//					parse_command
//					↓			↘
//	parse_simple_command	parse_expression (only for '(')

// Consumes && TK_OP_AND and || TK_OP_OR tokens
t_node	*parse_expression(t_ctx_parser *ctx)
{
	t_node			*node;
	t_token_type	token_op;
	t_node_type		node_op;

	node = parse_pipeline(ctx);
	while (node && ctx->arr_token[ctx->i].type & (TK_OP_AND | TK_OP_OR))
	{
		token_op = ctx->arr_token[ctx->i].type;
		ctx->i++;
		if (token_op == TK_OP_AND)
			node_op = NODE_AND;
		else if (token_op == TK_OP_OR)
			node_op = NODE_OR;
		node = new_node(node_op, node, parse_pipeline(ctx));
		if (!node)
			return (NULL);
		if (node->left == NULL || node->right == NULL)
			clean_nodes(&node);
	}
	return (node);
}

// Consumes | TK_PIPE tokens
t_node	*parse_pipeline(t_ctx_parser *ctx)
{
	t_node	*node;

	node = parse_command(ctx);
	while (node && ctx->arr_token[ctx->i].type == TK_PIPE)
	{
		ctx->i++;
		node = new_node(NODE_PIPE, node, parse_command(ctx));
		if (!node)
			return (NULL);
		if (node->left == NULL || node->right == NULL)
			clean_nodes(&node);
	}
	return (node);
}

t_node	*new_subshell(t_node *node)
{
	if (node == NULL)
		return (node);
	node = new_node(NODE_SUBSHELL, node, NULL);
	return (node);
}

// Consumes ( TK_OPEN_PARENT tokens
// if (ctx->arr_token[ctx->i].type == TK_END)
// HANDLE TRAILING OPERATORS
t_node	*parse_command(t_ctx_parser *ctx)
{
	t_node	*node;
	t_token	curr_token;

	if (ctx->arr_token[ctx->i].type == TK_END)
	{
		if (read_more_input(ctx) == -1)
			return (NULL);
	}
	curr_token = ctx->arr_token[ctx->i];
	if (curr_token.type == TK_OPEN_PARENT)
	{
		ctx->i++;
		node = parse_expression(ctx);
		if (ctx->arr_token[ctx->i].type == TK_CLOSE_PARENT)
			return (ctx->i++, new_subshell(node));
		else
			return (print_red("missing closing parenthesis."), NULL);
	}
	node = parse_simple_command(ctx);
	return (node);
}

//    █████████   █████      █████████  ███████████                            
//   ███▒▒▒▒▒███ ▒▒███      ███▒▒▒▒▒███▒█▒▒▒███▒▒▒█                            
//  ▒███    ▒███  ▒███████ ▒███    ▒▒▒ ▒   ▒███  ▒  ████████   ██████   ██████ 
//  ▒███████████  ▒███▒▒███▒▒█████████     ▒███    ▒▒███▒▒███ ███▒▒███ ███▒▒███
//  ▒███▒▒▒▒▒███  ▒███ ▒███ ▒▒▒▒▒▒▒▒███    ▒███     ▒███ ▒▒▒ ▒███████ ▒███████ 
//  ▒███    ▒███  ▒███ ▒███ ███    ▒███    ▒███     ▒███     ▒███▒▒▒  ▒███▒▒▒  
//  █████   █████ ████████ ▒▒█████████     █████    █████    ▒▒██████ ▒▒██████ 
// ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒    ▒▒▒▒▒      ▒▒▒▒▒▒   ▒▒▒▒▒▒  

t_node	*get_abstract_syntax_tree(char *str, t_shell *shell)
{
	t_ctx_parser	ctx;
	t_node			*node;

	ctx.arr_token = tokenize(str);
	ctx.i = 0;
	if (!ctx.arr_token)
		return (NULL);
	ctx.shell = shell;
	node = parse_expression(&ctx);
	if (node && ctx.arr_token[ctx.i].type != TK_END)
	{
		printf(RED"unexpected token `%s`"RESET"\n", ctx.arr_token[ctx.i].str);
		clean_nodes(&node);
	}
	clean_tokens(&ctx.arr_token);
	return (node);
}
