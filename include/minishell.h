/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:04:13 by wiwu              #+#    #+#             */
/*   Updated: 2026/01/14 16:44:34 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define RESET "\033[0m"
# define BOLD "\033[1m"

# include "libft.h"
# include "minishell_parse.h"
# include "minishell_signal.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <limits.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_ctx_parser	*ctx;
	t_node			*node;
	char			**envp;
	t_env			*env;
	int				last_exit;
}	t_shell;

t_shell	*init_shell(char **envp);
t_env	*init_env(char **envp);

t_env	*find_env_node(t_env *env, char *key);
void	set_env_value(t_shell *shell, char *key, char *value);
t_env	*new_env_node(char *key, char *value);
void	add_env_node(t_env **env, t_env *new);
char	*extract_key(char *s);
char	*extract_value(char *s);

int		is_var_char(char c);
char	*extract_var_name(char *s, int i);
char	*get_var_value(t_shell *shell, char *var_name);
char	*append_to_result(char *result, char *to_add);
void	update_quote_state(char c, int *in_single, int *in_double);

void	free_argv(char **argv);
int		expand_cmd_node(t_node *node, t_shell *shell);

char	*remove_quotes(char *s);
char	*expand_variable(char *s, t_shell *shell, int ignore_quotes);
void	handle_empty_args(char ***argv);

int		has_wildcard(char *str);
int		match_pattern(char *str, char *pattern);
int		count_matches(char *pattern);
void	fill_matches(char *pattern, char **matches);
void	sort_matches(char **matches);
int		count_total_args(char **argv);
int		count_matches_for_arg(char *arg);
char	**expand_single_wildcard(char *arg);
char	**expand_wildcards_in_argv(char **argv);

int		count_env(t_env *env);
char	**env_to_array(t_shell *shell);
char	*get_env_value(t_shell *shell, char *key);
void	unset_env_value(t_shell *shell, char *key);

int		ft_cd(char **args, t_shell *shell);
int		ft_echo(char **args);
int		ft_env(t_shell *shell);
int		ft_export(char **args, t_shell *shell);
int		ft_pwd(t_shell *shell);
int		ft_unset(char **args, t_shell *shell);
int		ft_exit(char **args, t_shell *shell);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell *shell);
int		execute_builtin_redir(char **argv, t_redir *redir, t_shell *shell);

int		execute_simple_command(t_node *node, t_shell *shell);
int		handle_redirections(t_redir *redir);
int		execute_pipeline(t_node *node, t_shell *shell);
int		execute_tree(t_node *node, t_shell *shell);

void	free_split(char **split);
char	*try_path(char *dir, char *cmd);
char	*get_path(char *cmd, t_shell *shell);
int		get_exit_status(int status);

void	clean_shell(t_shell *shell);
void	clean_env(t_env *env);
void	clean_nodes(t_node **node_addr);
void	clean_stuff(t_token **arr_token, t_node **node, t_shell *shell,
			int flag);

#endif
