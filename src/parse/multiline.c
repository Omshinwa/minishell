/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/11 20:58:41 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"
#include "minishell.h"
#include "minishell_signal.h"

// This read what's in the standard input and write to FD
void	read_heredoc_fd(int fd, char *del, int do_expand_vars, t_shell *shell)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("\001" BG_MAGENTA "\002" "<<" "\001" RESET "\002 ");
		if (!line)
		{
			printf(RED"here-doc: expected %s got EOF instead"RESET"\n", del);
			break ;
		}
		if (ft_strcmp(line, del) == 0)
			break ;
		if (do_expand_vars)
			expanded_line = expand_strings_options(line, shell, 0, 1);
		else
			expanded_line = ft_strdup(line);
		if (!expanded_line)
			break ;
		free(line);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
	}
	free(line);
}

// Fork a heredoc, close the write fd
int	fork_heredoc_child(t_node *node, t_ctx_parser *ctx, char *delimiter
	, int fd[2])
{
	int		pid;
	char	*expanded_del;

	if (ft_is_in_set(delimiter, '"') || ft_is_in_set(delimiter, '\''))
		expanded_del = expand_strings_options(delimiter, ctx->shell, 1, 0);
	else
		expanded_del = ft_strdup(delimiter);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		set_signals_multilines();
		read_heredoc_fd(fd[1], expanded_del,
			!ft_strcmp(delimiter, expanded_del), ctx->shell);
		free(expanded_del);
		close(fd[1]);
		clean_tokens(&ctx->arr_token);
		clean_shell(ctx->shell);
		clean_nodes(&node);
		exit(0);
	}
	else if (pid == -1)
		return (print_red("pid"), -1);
	return (0);
}

// prints a pretty prompt when && || etc
static char	*new_prompt(t_ctx_parser *ctx)
{
	char	*prompt;
	char	*color;

	if (ctx->arr_token[ctx->i - 1].type == TK_OP_AND)
		color = BG_GREEN;
	else if (ctx->arr_token[ctx->i - 1].type == TK_OP_OR)
		color = BG_YELLOW;
	else if (ctx->arr_token[ctx->i - 1].type == TK_PIPE)
		color = BG_RED;
	else if (ctx->arr_token[ctx->i - 1].type == TK_OPEN_PARENT)
		color = BG_BLUE;
	prompt = safe_malloc(sizeof(char) * 32);
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	ft_strlcat(prompt, "\001", 32);
	ft_strlcat(prompt, color, 32);
	ft_strlcat(prompt, "\002", 32);
	ft_strlcat(prompt, ctx->arr_token[ctx->i - 1].str, 32);
	ft_strlcat(prompt, "\001", 32);
	ft_strlcat(prompt, RESET, 32);
	ft_strlcat(prompt, "\002", 32);
	ft_strlcat(prompt, " ", 32);
	return (prompt);
}

static int	not_just_empty_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

// When we unexpectedly have a TK_END, we read more input from the user.
// We tokenize the new input, and change the context's array of token to the
// new array and set the index to 0.
// Return 0 on success, -1 on error
// set_signals_multilines() if we used fork, but we don't
int	command_continuation(t_ctx_parser *ctx)
{
	char	*line;
	t_token	*new_tokens;
	char	*prompt;

	prompt = new_prompt(ctx);
	if (!prompt)
		return (-1);
	while (1)
	{
		line = readline(prompt);
		if (!line)
			return (free(prompt), print_red("syntax: unexpected EOF"), -1);
		if (not_just_empty_space(line))
			break ;
		free(line);
	}
	free(prompt);
	new_tokens = tokenize(line);
	free(line);
	if (!new_tokens)
		return (-1);
	clean_tokens(&ctx->arr_token);
	ctx->arr_token = new_tokens;
	ctx->i = 0;
	return (0);
}
