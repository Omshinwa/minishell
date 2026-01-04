/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/04 23:10:57 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

//  ███████████    ███████    █████   ████ ██████████ ██████   █████  █████████ 
// ▒█▒▒▒███▒▒▒█  ███▒▒▒▒▒███ ▒▒███   ███▒ ▒▒███▒▒▒▒▒█▒▒██████ ▒▒███  ███▒▒▒▒▒███
// ▒   ▒███  ▒  ███     ▒▒███ ▒███  ███    ▒███  █ ▒  ▒███▒███ ▒███ ▒███    ▒▒▒ 
//     ▒███    ▒███      ▒███ ▒███████     ▒██████    ▒███▒▒███▒███ ▒▒█████████ 
//     ▒███    ▒███      ▒███ ▒███▒▒███    ▒███▒▒█    ▒███ ▒▒██████  ▒▒▒▒▒▒▒▒███
//     ▒███    ▒▒███     ███  ▒███ ▒▒███   ▒███ ▒   █ ▒███  ▒▒█████  ███    ▒███
//     █████    ▒▒▒███████▒   █████ ▒▒████ ██████████ █████  ▒▒█████▒▒█████████ 
//    ▒▒▒▒▒       ▒▒▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒  

// (|, <, >, &, (, ), espace).
static int	is_metachar(char c)
{
	if (ft_isalnum(c))
		return (0);
	if (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')' || c == ' ' || c == '"' || c == '\'')
		return (1);
	if (c == ';' || c == '\\')
		return (1);
	return (0);
}

void	clean_tokens(t_token **arr_token)
{
	int	i;

	i = 0;
	while ((*arr_token)[i].type != TK_END)
	{
		free((*arr_token)[i].str);
		i++;
	}
	free(*arr_token);
	*arr_token = NULL;
}

// Given the string to parse and the current index, 
// Set the token type, return END as the new index
// returns -1 on error
static int	set_token(t_token *token, char *str, int start)
{
	int	end;
	int	c;

	end = start;
	c = str[start];
	token->str = NULL;
	if (c == '!') // DO INVALID CHARS HERE
		return (token->type = TK_END, -1);
	else if (!is_metachar(c) || c == '"' || c == '\'')
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
			else if (!is_metachar(str[end]))
				end++;
			else
				break ;
		}
	}
	else if (c == '(')
	{
		token->type = TK_OPEN_PARENT;
		end++;
	}
	else if (c == ')')
	{
		token->type = TK_CLOSE_PARENT;
		end++;
	}
	else if (c == '|')
	{
		token->type = TK_PIPE;
		end++;
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
			end += 2;
		}
		else
			return (token->type = TK_END, -1);
	}
	else if (c == '<')
	{
		token->type = TK_REDIR_IN;
		end++;
		if (str[end] == '<')
		{
			token->type = TK_REDIR_2IN;
			end++;
		}
	}
	else if (c == '>')
	{
		token->type = TK_REDIR_OUT;
		end++;
		if (str[end] == '>')
		{
			token->type = TK_REDIR_2OUT;
			end++;
		}
	}
	else
		return (token->type = TK_END, print_red("invalid char"), -1);
	token->str = ft_substr(str, start, end - start);
	return (end);
}

// Goes through the string input,
// Returns an array of tokens.
/// TODO: could optimize the calloc, im doing the worst case scenario.
t_token	*tokenize(char *str)
{
	int		i;
	int		new_i;
	int		j;
	t_token	*arr_token;

	i = 0;
	j = 0;
	arr_token = safe_malloc(sizeof(t_token) * (ft_strlen(str) + 1));
	if (!arr_token)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\n')
		{
			i++;
			continue ;
		}
		arr_token[j] = (t_token){NULL, TK_END};
		new_i = set_token(&arr_token[j], str, i);
		if (new_i == -1)
		{
			printf(RED "invalid token at index %i: %c" RESET "\n", i, str[i]);
			break ;
		}
		i = new_i;
		j++;
	}
	arr_token[j] = (t_token){NULL, TK_END};
	if (new_i == -1)
		clean_tokens(&arr_token);
	return (arr_token);
}

void	print_tokens(t_token *arr_token)
{
	int		i;
	t_token	token;

	i = -1;
	if (!arr_token)
		return ;
	while (arr_token[++i].type != TK_END)
	{
		token = arr_token[i];
		printf(BOLD);
		if (token.type == TK_WORD)
			printf(BG_WHITE BLACK "%s" RESET, token.str);
		else if (token.type == TK_PIPE)
			printf(BG_RED "%s" RESET, token.str);
		else if (token.type == TK_REDIR_IN || token.type == TK_REDIR_OUT)
			printf(BG_MAGENTA "%s" RESET, token.str);
		else if (token.type == TK_REDIR_2IN || token.type == TK_REDIR_2OUT)
			printf(BG_MAGENTA "%s" RESET, token.str);
		else if (token.type == TK_OP_AND)
			printf(BG_GREEN "%s" RESET, token.str);
		else if (token.type == TK_OP_OR)
			printf(BG_YELLOW "%s" RESET, token.str);
		else if (token.type == TK_OPEN_PARENT || token.type == TK_CLOSE_PARENT)
			printf(BG_BLUE "%s" RESET, token.str);
		printf(" ");
	}
	printf("\n");
}
