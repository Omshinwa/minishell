/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_setter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 20:23:51 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

// (|, <, >, &, (, ), espace).
static int	is_valid_word_char(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')' || c == ' ' || c == '"' || c == '\'')
		return (0);
	if (c == ';' || c == '\\')
		return (0);
	return (1);
}

static int	set_word_token(t_token *token, char *str, char c, int end)
{
	token->type = TK_WORD;
	while (str[end])
	{
		if (str[end] == '"' || str[end] == '\'')
		{
			c = str[end];
			end++;
			while (str[end] && str[end] != c)
				end++;
			if (str[end] == c)
				end++;
			else
			{
				token->type = TK_END;
				print_red("unclosed quotation");
				return (-1);
			}
		}
		else if (is_valid_word_char(str[end]))
			end++;
		else
			break ;
	}
	return (end);
}

static int	set_token_operations(t_token *token, char *str, char c, int end)
{
	end++;
	if (c == '(')
		token->type = TK_OPEN_PARENT;
	else if (c == ')')
		token->type = TK_CLOSE_PARENT;
	else if (c == '|')
	{
		token->type = TK_PIPE;
		if (str[end] == '|')
		{
			token->type = TK_OP_OR;
			end++;
		}
	}
	else if (c == '&')
	{
		if (str[end + 1] == '&')
		{
			token->type = TK_OP_AND;
			end += 1;
		}
		else
			return (token->type = TK_END, -1);
	}
	return (end);
}

static int	set_token_else(t_token *token, char *str, char c, int end)
{
	if (c == '<')
	{
		end++;
		token->type = TK_REDIR_IN;
		if (str[end] == '<')
		{
			token->type = TK_REDIR_2IN;
			end++;
		}
	}
	else if (c == '>')
	{
		end++;
		token->type = TK_REDIR_OUT;
		if (str[end] == '>')
		{
			token->type = TK_REDIR_2OUT;
			end++;
		}
	}
	else
		return (token->type = TK_END, print_red("invalid char"), -1);
	return (end);
}

// Given the string to parse and the current index, 
// Set the token type, return END as the new index
// returns -1 on error
// --> if (c == '!') 	///		use this line to set invalid chars
int	set_token(t_token *token, char *str, int start)
{
	int	end;
	int	c;

	c = str[start];
	end = start;
	token->str = NULL;
	if (c == '!')
		return (token->type = TK_END, -1);
	else if (is_valid_word_char(c) || c == '"' || c == '\'')
		end = set_word_token(token, str, c, end);
	else if (c == '(' || c == ')' || c == '|' || c == '&')
		end = set_token_operations(token, str, c, end);
	else
		end = set_token_else(token, str, c, end);
	if (end == -1)
		return (-1);
	token->str = ft_substr(str, start, end - start);
	return (end);
}
