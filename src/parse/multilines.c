/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multilines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 11:11:46 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"
#include "minishell.h"
#include "minishell_signal.h"

// This read what's in the standard input and write to FD
int	read_heredoc_fd(int fd, char *del)
{
	char	*line;

	while (1)
	{
		line = readline("\001" BG_MAGENTA "\002" "<<" "\001" RESET "\002 ");
		if (!line)
		{
			printf(RED"here-doc: expected %s got EOF instead"RESET"\n", del);
			break ;
		}
		if (ft_strcmp(line, del) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	exit(0);
}

static char	*new_prompt(t_parser *ctx)
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
	prompt = safe_malloc(sizeof(char) * 40);
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	ft_strlcat(prompt, "\001", 50);
	ft_strlcat(prompt, color, 50);
	ft_strlcat(prompt, "\002", 50);
	ft_strlcat(prompt, ctx->arr_token[ctx->i - 1].str, 50);
	ft_strlcat(prompt, "\001", 50);
	ft_strlcat(prompt, RESET, 50);
	ft_strlcat(prompt, "\002", 50);
	ft_strlcat(prompt, " ", 50);
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
int	read_more_input(t_parser *ctx)
{
	char	*line;
	t_token	*new_tokens;
	char	*prompt;

	// set_signals_multilines();
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
