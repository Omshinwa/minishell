/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parse.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/08 19:16:13 by wiwu             ###   ########.fr       */
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

// typedef enum e_state {
// 	STATE_INTERACTIVE,
// 	STAGE_HEREDOC,
// 	STATE_EXEC
// }	t_state;

// typedef struct s_shell
// {
// 	t_state	state;
// 	t_env	*env;
// 	int		last_exit;
// }	t_shell;

// 111111111111 - 12 bits
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
} t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
} t_token;

typedef struct s_parser
{
	t_token *arr_token;
	int     i;
} t_parser;

typedef enum e_node_type {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
} t_node_type;

typedef struct s_redir {
	t_token_type	type;	// TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_2IN, TK_REDIR_2OUT
	char *filename;			// file or heredoc delimiter
	int	fd;
	struct s_redir *next;
} t_redir;

typedef struct s_node {
	t_node_type type;
	char **argv;
	t_redir *redir;
	struct s_node *left;
	struct s_node *right;
} t_node;

// multilines.c
int	read_heredoc_fd(int fd, char *del);
int	read_more_input(t_parser *ctx);

// token.c
t_token	*tokenize(char *str);
void	print_tokens(t_token *arr_token);
void	clean_tokens(t_token **arr_token);
void	print_nodes(t_node *node, int depth);

// node.c
t_node *new_node(t_node_type type, t_node *left, t_node *right);
void	clean_nodes(t_node **node);

t_node	*get_abstract_syntax_tree(char *str);

// recursive calls
t_node *parse_expression(t_parser *ctx);
t_node *parse_pipeline(t_parser *ctx);
t_node *parse_command(t_parser *ctx);
t_node	*parse_simple_command(t_parser *ctx);

void	print_red(char *str);
void	*safe_malloc(size_t __size);

// currently signals are pretty dirty, need some rework
void	signal_handler(int signum);

#endif
