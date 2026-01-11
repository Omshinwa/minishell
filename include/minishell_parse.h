/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parse.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/11 14:52:02 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSE_H
# define MINISHELL_PARSE_H

# define BLACK   "\033[1;30m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

# define RESET "\033[0m"
# define BOLD  "\033[1m"

# define BG_BLACK   "\033[40m"
# define BG_RED     "\033[41m"
# define BG_GREEN   "\033[42m"
# define BG_YELLOW  "\033[43m"
# define BG_BLUE    "\033[44m"
# define BG_MAGENTA "\033[45m"
# define BG_CYAN    "\033[46m"
# define BG_WHITE   "\033[47m"

# include "libft.h"
# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

# include <readline/readline.h>
# include <readline/history.h>

# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_shell	t_shell;

typedef enum e_token_type {
	TK_WORD = 1,
	TK_PIPE = 2,
	TK_REDIR_IN = 4,
	TK_REDIR_OUT = 8,
	TK_REDIR_2IN = 16,
	TK_REDIR_2OUT = 32,
	TK_OP_AND = 64,
	TK_OP_OR = 128,
	TK_OPEN_PARENT = 256,
	TK_CLOSE_PARENT = 512,
	TK_END = 1024,
}	t_token_type;

typedef enum e_node_type {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
}	t_node_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
}	t_token;

// Linked list of redirections
// FILENAME has a file name or heredoc delimiter
// FD is only valid for TK_REDIR_2IN (heredoc)
typedef struct s_redir {
	t_token_type	type;
	char			*filename;
	int				fd;
	struct s_redir	*next;
}	t_redir;

// ARGV and REDIR are only valid for CMD nodes
typedef struct s_node {
	t_node_type		type;
	char			**argv;
	t_redir			*redir;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_ctx_parser
{
	t_shell	*shell;
	t_token	*arr_token;
	int		i;
}	t_ctx_parser;

// multilines.c
int		command_continuation(t_ctx_parser *ctx);
int		fork_heredoc(t_node *node, t_ctx_parser *ctx,
			t_redir *redir, int fd[2]);
int		fork_heredoc_child(t_node *node, t_ctx_parser *ctx, char *delimiter,
			int fd[2]);
// token.c
void	print_tokens(t_token *arr_token);
void	clean_tokens(t_token **arr_token);
void	print_nodes(t_node *node, int depth);
t_token	*tokenize(char *str);
int		set_token(t_token *token, char *str, int start);

// node.c
t_node	*new_node(t_node_type type, t_node *left, t_node *right);
void	clean_nodes(t_node **node);

t_node	*get_abstract_syntax_tree(char *str, t_shell *shell);

// recursive calls
t_node	*parse_expression(t_ctx_parser *ctx);
t_node	*parse_pipeline(t_ctx_parser *ctx);
t_node	*parse_command(t_ctx_parser *ctx);
t_node	*parse_simple_command(t_ctx_parser *ctx);

void	print_red(char *str);
void	*safe_malloc(size_t __size);

#endif
