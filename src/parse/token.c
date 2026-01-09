/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 20:26:35 by wiwu             ###   ########.fr       */
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

static int	populate_token(char *str, t_token *arr_token, int *size)
{
	int		i;
	int		valid_token;

	i = 0;
	valid_token = -1;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\n')
		{
			i++;
			continue ;
		}
		arr_token[*size] = (t_token){NULL, TK_END};
		valid_token = set_token(&arr_token[*size], str, i);
		if (valid_token == -1)
			return (printf(RED "invalid token at index %i: %c" RESET "\n",
					i, str[i]), -1);
		i = valid_token;
		(*size)++;
	}
	return (0);
}

// Goes through the string input, Returns an array of tokens.
// logic: j is the length of tokens, i for str
// new_i is the new index after setting a token
/// TODO: could optimize the calloc, im doing the worst case scenario.
t_token	*tokenize(char *str)
{
	int		i;
	int		size;
	t_token	*arr_token;
	int		failed;

	i = 0;
	size = 0;
	failed = 0;
	arr_token = safe_malloc(sizeof(t_token) * (ft_strlen(str) + 1));
	if (!arr_token)
		return (NULL);
	if (populate_token(str, arr_token, &size))
		failed = 1;
	arr_token[size] = (t_token){NULL, TK_END};
	if (failed || size == 0)
		clean_tokens(&arr_token);
	return (arr_token);
}

// void	print_tokens(t_token *arr_token)
// {
// 	int		i;
// 	t_token	token;

// 	i = -1;
// 	if (!arr_token)
// 		return ;
// 	while (arr_token[++i].type != TK_END)
// 	{
// 		token = arr_token[i];
// 		printf(BOLD);
// 		if (token.type == TK_WORD)
// 			printf(BG_WHITE BLACK "%s" RESET, token.str);
// 		else if (token.type == TK_PIPE)
// 			printf(BG_RED "%s" RESET, token.str);
// 		else if (token.type == TK_REDIR_IN || token.type == TK_REDIR_OUT)
// 			printf(BG_MAGENTA "%s" RESET, token.str);
// 		else if (token.type == TK_REDIR_2IN || token.type == TK_REDIR_2OUT)
// 			printf(BG_MAGENTA "%s" RESET, token.str);
// 		else if (token.type == TK_OP_AND)
// 			printf(BG_GREEN "%s" RESET, token.str);
// 		else if (token.type == TK_OP_OR)
// 			printf(BG_YELLOW "%s" RESET, token.str);
// 		else if (token.type == TK_OPEN_PARENT || token.type == TK_CLOSE_PARENT)
// 			printf(BG_BLUE "%s" RESET, token.str);
// 		printf(" ");
// 	}
// 	printf("\n");
// }
