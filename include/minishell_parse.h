/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parse.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/02 06:12:33 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	TK_WORD,
	TK_PIPE,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_REDIR_ININ,
	TK_REDIR_OUTOUT,
	TK_OP_AND,
	TK_OP_OR,
	TK_OPEN_PARENT,
	TK_CLOSE_PARENT,
	TK_END,
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
} t_token;

typedef struct s_shell
{
	int				err;
}	t_shell;

typedef enum e_node_type {
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
} t_node_type;

typedef struct s_redir {
	int type;       // TK_REDIR_IN, TK_REDIR_OUT, REDIR_APPEND, HEREDOC
	char *filename; // file or heredoc content
	struct s_redir *next;
} t_redir;

typedef struct s_node {
	t_node_type type;
	char **argv;
	t_redir *redir;
	struct s_node *left;
	struct s_node *right;
} t_node;



void	print_red(char *str);

#endif
