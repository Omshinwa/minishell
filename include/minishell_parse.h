/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parse.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2025/12/30 17:20:11 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define BLACK   "\033[1;30m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

#define RESET "\033[0m"
#define BOLD  "\033[1m"

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

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

typedef enum e_enum_token {
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_ININ,
	REDIR_OUTOUT,
	AND,
	OR,
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	END,
} t_enum_token;

typedef struct s_token
{
	char			*str;
	t_enum_token	type;
} t_token;

typedef struct s_shell
{
	int				err;
}	t_shell;

typedef struct s_node
{
	char	**cmd_args;
	char	*in;
	char	*out;
} t_node;

void	print_err(t_shell *shell, char *str);

#endif
