/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2025/12/30 17:18:27 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

// Peut pas y a voir plus de tokens que le nombre d'espaces.

// // Goes through the string and count the number of tokens, used for the malloc.
// size_t	count_tokens(char *str)
// {
// 	size_t	res;
// 	size_t	i;

// 	res = 0;
// 	i = 0;

// 	while(str[i])
// 	{
// 		if (s[i] != c && (i == 0 || s[i - 1] == c))
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// give the first char of a new token, 
// change the token type?
// return i as the progress made
int	set_token(t_token *token, char *str, int start)
{
	// WORD
	int end_offset; // sometimes we shave from the end
	int	end; // return
	int	c;

	end_offset = 0;
	end = start;
	c = str[start];
	if (ft_isalnum(c)) // until space
	{
		token->type = WORD;
		while(str[end] && ft_isalnum(str[end]))
			end++;
	}
	else if (c == '-')
	{
		while (str[end] == '-')
			end++;
		if (ft_isalnum(c)) // until space
		{
			token->type = WORD;
			while(str[end] && ft_isalnum(str[end]))
				end++;
		}
		else
			return (-1);
	}
	else if (c == '"' || c == '\'') // until quotation mark
	{
		start++;
		token->type = WORD;
		end++;
		while(str[end] && str[end] != c)
			end++;
		if (str[end] == c) // found matching quote mark
			end_offset = 1;
		else // reached end of str
			return (-1);
	}
	// PARENTHESIS
	else if (c == '(')
	{
		token->type = OPEN_PARENTHESIS;
		end++;
	}
	else if (c == ')')
	{
		token->type = CLOSE_PARENTHESIS;
		end++;
	}
	// PIPE
	else if (c == '|')
	{
		token->type = PIPE;
		end++;
		if (str[end] == '|')
		{
			token->type = OR;
			end++;
		}
	}
	else if (c == '&')
	{
		if (str[end + 1] == '&')
		{
			token->type = AND;
			end += 2;
		}
		else
			return (-1);
	}
	// REDIR_IN, REDIR_OUT, REDIR_ININ, REDIR_OUTOUT
	else if (c == '<')
	{
		token->type = REDIR_IN;
		end++;
		if (str[end] == '<')
		{
			token->type = REDIR_ININ;
			end++;
		}
	}
	else if (c == '>')
	{
		token->type = REDIR_OUT;
		end++;
		if (str[end] == '>')
		{
			token->type = REDIR_OUTOUT;
			end++;
		}
	}
	else
		return (-1);
	token->str = ft_substr(str, start, end - start);
	return (end + end_offset);
}

// Goes through the string input,
// Returns an array of tokens.
t_token *tokenize(char *str)
{
	int i;
	int	new_i;
	int	j;
	t_token	*arr_token;

	i = 0;
	j = 0;
	arr_token = calloc(sizeof(t_token), ft_strlen(str) / 2 + 1);
	while (str[i])
	{
		if (str[i] == ' ')
		{
			i++;
			continue;
		}
		arr_token[j] = (t_token) {"", -1};
		new_i = set_token(&arr_token[j], str, i);
		if (new_i == -1)
		{
			printf(RED "invalid token at index %i : %s\n", i, &str[i]);
			print_err(NULL, NULL);
		}
		i = new_i;
		j++;
	}
	arr_token[j] = (t_token) {"", END};
	return (arr_token);
}

t_node	**parse(char *str)
{
	t_node	**node_list; 
	t_node	*current_node; 
	char	**str_list;
	int		i;
	int		len_arg;

	str_list = ft_split(str, '|');
	i = 0;
	while(str_list[i] != NULL)
		i++;
	len_arg = i;
	node_list = calloc(sizeof(t_node), len_arg);
	i = -1;
	while(++i < len_arg)
	{
		current_node = calloc(sizeof(t_node), 1);
		current_node->cmd_args = ft_split(str_list[i], ' ');
		node_list[i] = current_node;
	}
	node_list[i] = NULL;
	return (node_list);
}

void print_list_of_cmds(t_node **node_list)
{
	int	i;
	int	j;
	t_node	*node;

	i = -1;
	while(node_list[++i])
	{
		node = node_list[i];
		printf(BOLD "CMD: %s" RESET, node->cmd_args[0]);
		j = 0;
		while(node->cmd_args[++j])
		{
			printf(" arg%i: %s", j, node->cmd_args[j]);
		}
		printf("\n|\n");
	}
}

void print_list_of_tokens(t_token *arr_token)
{
	int	i;
	t_token	token;

	i = -1;
	while(arr_token[++i].type != END)
	{
		token = arr_token[i];
		printf(BOLD);
		if (token.type == WORD)
			printf(BG_WHITE BLACK "%s" RESET, token.str);
		else if (token.type == PIPE)
			printf(BG_RED "%s" RESET, token.str);
		else if (token.type == REDIR_IN || token.type == REDIR_OUT)
			printf(BG_GREEN "%s" RESET, token.str);
		else if (token.type == REDIR_ININ || token.type == REDIR_OUTOUT)
			printf(BG_YELLOW "%s" RESET, token.str);
		else if (token.type == AND || token.type == OR)
			printf(BG_BLUE "%s" RESET, token.str);
		else if (token.type == OPEN_PARENTHESIS || token.type == CLOSE_PARENTHESIS)
			printf(BG_MAGENTA "%s" RESET, token.str);
		printf(" ");
	}
	printf("\n");
}

volatile sig_atomic_t	g_ackowledge = 0;
// an atomic operation guarantees that every read/write from g_acknoledge are
// done in full before handling a signal.
// volatile prevents the compiler from making some optimizations

static void	acknowledge_sig(int signum)
{
	g_ackowledge = signum;
	// printf("%i received %i \n", getpid(), signum);
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
	}
}

int main(void)
{
	char *str;
	t_token	*arr_token; 

	signal(SIGINT, acknowledge_sig);
	signal(SIGQUIT, SIG_IGN);
	// fork();
	while(1)
	{
		str = readline("👻>");
		if (!str)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(str))
		{
			add_history(str);
			arr_token = tokenize(str);
			print_list_of_tokens(arr_token);
		}
		free(str);
	}
	return (0);
}
