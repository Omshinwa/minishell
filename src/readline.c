/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2025/12/31 22:44:05 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

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

//  ███████████    ███████    █████   ████ ██████████ ██████   █████  █████████ 
// ▒█▒▒▒███▒▒▒█  ███▒▒▒▒▒███ ▒▒███   ███▒ ▒▒███▒▒▒▒▒█▒▒██████ ▒▒███  ███▒▒▒▒▒███
// ▒   ▒███  ▒  ███     ▒▒███ ▒███  ███    ▒███  █ ▒  ▒███▒███ ▒███ ▒███    ▒▒▒ 
//     ▒███    ▒███      ▒███ ▒███████     ▒██████    ▒███▒▒███▒███ ▒▒█████████ 
//     ▒███    ▒███      ▒███ ▒███▒▒███    ▒███▒▒█    ▒███ ▒▒██████  ▒▒▒▒▒▒▒▒███
//     ▒███    ▒▒███     ███  ▒███ ▒▒███   ▒███ ▒   █ ▒███  ▒▒█████  ███    ▒███
//     █████    ▒▒▒███████▒   █████ ▒▒████ ██████████ █████  ▒▒█████▒▒█████████ 
//    ▒▒▒▒▒       ▒▒▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒  

// (|, <, >, &, (, ), espace).
int	is_metachar(char c)
{
	if (ft_isalnum(c))
		return (0);
	// those char are fine if in quotes, otherwise special behavior
	if (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')' || c == ' ' || c == '"' || c == '\'')
		return (1);
	// those char should always give an error:
	if (c == ';' || c == '\\')
		return (1);
	return (0);
}

// give the first char of a new token, 
// change the token type?
// return i as the progress made
/// TODO: expands $ variables
int	set_token(t_token *token, char *str, int start)
{
	// TK_WORD
	int	end; // return
	int	c;

	end = start;
	c = str[start];
	if (!is_metachar(c) || c == '"' ||  c == '\'') // until space
	{
		token->type = TK_WORD;
		while(str[end])
		{
			if (str[end] == '"' || str[end] == '\'') // until quotation mark
			{
				c = str[end];
				end++;
				while(str[end] && str[end] != c)
					end++;
				if (str[end] == c)
					end++;
				else // no matching quote mark
					return (print_red("unclosed quotation"), -1);
			}
			else if (!is_metachar(str[end]))
				end++;
			else
				break ;
		}
	}
	// PARENTHESIS
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
	// TK_PIPE
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
			return (print_red("invalid char: &"), -1);
	}
	// TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_ININ, TK_REDIR_OUTOUT
	else if (c == '<')
	{
		token->type = TK_REDIR_IN;
		end++;
		if (str[end] == '<')
		{
			token->type = TK_REDIR_ININ;
			end++;
		}
	}
	else if (c == '>')
	{
		token->type = TK_REDIR_OUT;
		end++;
		if (str[end] == '>')
		{
			token->type = TK_REDIR_OUTOUT;
			end++;
		}
	}
	else
		return (print_red("invalid char"),-1);
	token->str = ft_substr(str, start, end - start);
	return (end);
}

// Goes through the string input,
// Returns an array of tokens.
/// TODO: could optimize the calloc, im doing the worst case scenario.
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
			printf(RED "invalid token at index %i : %c\n", i, str[i]);
		}
		i = new_i;
		j++;
	}
	arr_token[j] = (t_token) {"", TK_END};
	return (arr_token);
}

void print_tokens(t_token *arr_token)
{
	int	i;
	t_token	token;

	i = -1;
	while(arr_token[++i].type != TK_END)
	{
		token = arr_token[i];
		printf(BOLD);
		if (token.type == TK_WORD)
			printf(BG_WHITE BLACK "%s" RESET, token.str);
		else if (token.type == TK_PIPE)
			printf(BG_RED "%s" RESET, token.str);

		else if (token.type == TK_REDIR_IN || token.type == TK_REDIR_OUT)
			printf(BG_GREEN "%s" RESET, token.str);
		else if (token.type == TK_REDIR_ININ || token.type == TK_REDIR_OUTOUT)
			printf(BG_YELLOW "%s" RESET, token.str);
	
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

//  ███████████    █████████   ███████████    █████████  ██████████
// ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███ ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███▒▒███▒▒▒▒▒█
//  ▒███    ▒███ ▒███    ▒███  ▒███    ▒███ ▒███    ▒▒▒  ▒███  █ ▒ 
//  ▒██████████  ▒███████████  ▒██████████  ▒▒█████████  ▒██████   
//  ▒███▒▒▒▒▒▒   ▒███▒▒▒▒▒███  ▒███▒▒▒▒▒███  ▒▒▒▒▒▒▒▒███ ▒███▒▒█   
//  ▒███         ▒███    ▒███  ▒███    ▒███  ███    ▒███ ▒███ ▒   █
//  █████        █████   █████ █████   █████▒▒█████████  ██████████
// ▒▒▒▒▒        ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒▒ 

//    █████████   █████      █████████  ███████████                            
//   ███▒▒▒▒▒███ ▒▒███      ███▒▒▒▒▒███▒█▒▒▒███▒▒▒█                            
//  ▒███    ▒███  ▒███████ ▒███    ▒▒▒ ▒   ▒███  ▒  ████████   ██████   ██████ 
//  ▒███████████  ▒███▒▒███▒▒█████████     ▒███    ▒▒███▒▒███ ███▒▒███ ███▒▒███
//  ▒███▒▒▒▒▒███  ▒███ ▒███ ▒▒▒▒▒▒▒▒███    ▒███     ▒███ ▒▒▒ ▒███████ ▒███████ 
//  ▒███    ▒███  ▒███ ▒███ ███    ▒███    ▒███     ▒███     ▒███▒▒▒  ▒███▒▒▒  
//  █████   █████ ████████ ▒▒█████████     █████    █████    ▒▒██████ ▒▒██████ 
// ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒    ▒▒▒▒▒      ▒▒▒▒▒▒   ▒▒▒▒▒▒  


// while (token_is_redirection(arr_token[*index]))
// {
//     parse_redirection(node, arr_token, index);
// }

// Create a new node in the AST
t_node *new_node(t_node_type type, t_node *left, t_node *right)
{
	t_node *node = malloc(sizeof(t_node));
	if (!node)
		return (print_red("malloc"), NULL); // handle malloc failure
	node->type = type; // e.g., TK_AND → NODE_AND
	node->argv = NULL;                   // only for CMD nodes
	node->redir = NULL;                  // only for CMD nodes
	node->left = left;
	node->right = right;
	return node;
}

t_node_type token_to_node_type(t_token_type type)
{
	if (type == TK_WORD)
		return (NODE_CMD);
	else if (type == TK_PIPE)
		return (NODE_PIPE);
	else if (type == TK_OP_AND)
		return (NODE_AND);
	else if (type == TK_OP_OR)
		return (NODE_OR);
	else if (type == TK_OPEN_PARENT)
		return (NODE_SUBSHELL);
	return (print_red("unrecognized token"), -1);
}

// parse_expression
//    ↓
// parse_pipeline
//    ↓
// parse_command
//         ↘
//          parse_expression (only for '(')

void	print_nodes(t_node *node, int depth)
{
	char *color;
	int	i;
	char	offset[depth * 7 + 1];

	i = 0;
	while (i < depth * 7)
	{
		offset[i] = ' ';
		i++;
	}
	offset[i] = '\0';
	i = 1;
	color = BG_WHITE BOLD;
	if (node->type == NODE_CMD)
	{
		i = 0;
		while (node->argv[i] != NULL && i < 5)
		{
			if (i == 4 && node->argv[5])
				printf("%s%s %13s" RESET "\n", offset, color, "...");
			else if (i == 0)
				printf("%s%s>CMD:%9s" RESET "\n", offset, color, node->argv[i]);
			else
				printf("%s%s%14s" RESET "\n", offset, color, node->argv[i]);
			i++;
		}
	}
	else if (node->type == NODE_PIPE)
	{
		color = BG_RED;
		printf("%s%s>PIPE: %7s" RESET "\n", offset, color, "|");
	}
	else if (node->type == NODE_AND)
	{
		color = BG_GREEN;
		printf("%s%s>AND: %8s" RESET "\n", offset, color, "&&");
	}
	else if (node->type == NODE_OR)
	{
		color = BG_YELLOW;
		printf("%s%s>OR: %9s" RESET "\n", offset, color, "&&");
	}
	else if (node->type == NODE_SUBSHELL)
	{
		color = BG_BLUE;
		printf("%s%s>%13s" RESET "\n", offset, color, "SUBSHELL     ");
	}
	while (i++ < 5)
		printf("%s%s %13s" RESET "\n", offset, color, " ");
	if (node->left)
		print_nodes(node->left, depth + 1);
	if (node->right)
		print_nodes(node->right, depth + 1);
}

t_node	*parse_simple_command(t_token *arr_token, int *index);
t_node *parse_expression(t_token *arr_token, int *index);
t_node *parse_pipeline(t_token *arr_token, int *index);
t_node *parse_command(t_token *arr_token, int *index);

t_node *parse_tokens(t_token *arr_token)
{
	int	i;

	i = 0;
	return (parse_expression(arr_token, &i));
}

t_node *parse_expression(t_token *arr_token, int *index)
{
	t_node *node;
	t_token_type	op;

	node = parse_pipeline(arr_token, index);
	while (arr_token[*index].type == TK_OP_AND || arr_token[*index].type == TK_OP_OR)
	{
		op = arr_token[*index].type;
		(*index)++;
		node = new_node(token_to_node_type(op),
			node, parse_pipeline(arr_token, index));
	}
	return node;
}

// Should never have index pointing at an && or || token.
t_node *parse_pipeline(t_token *arr_token, int *index)
{
	t_node *node;

	node = parse_command(arr_token, index);
	while (arr_token[*index].type == TK_PIPE)
	{
		(*index)++;
		node = new_node(NODE_PIPE, node, parse_command(arr_token, index));
	}
	return node;
}

t_node *new_subshell(t_node *node)
{
	node = new_node(NODE_SUBSHELL, node, NULL);
	return (node);
}

// Should never have index pointing at an &&, ||, or pipe token.
t_node *parse_command(t_token *arr_token, int *index)
{
	t_node *node;
	t_token curr_token;

	curr_token = arr_token[*index];
	if (curr_token.type == TK_OPEN_PARENT)
	{
		(*index)++;
		node = parse_expression(arr_token, index);
		if (arr_token[*index].type == TK_CLOSE_PARENT)
			return ((*index)++, new_subshell(node));
		else
			return(print_red("missing closing parenthesis."), NULL);
		// if (arr_token[*index].type == TK_END)
		// 	return(print_red("missing closing parenthesis."), NULL);
		// node = parse_expression(arr_token, index);
	}
	return parse_simple_command(arr_token, index);
}

// Should never have index pointing at an &&, ||, pipe or () token.
t_node	*parse_simple_command(t_token *arr_token, int *index)
{
	t_node *node;
	t_token curr_token;
	int	nb_of_consecutive_words;

	node = new_node(NODE_CMD, NULL, NULL);
	nb_of_consecutive_words = 0;
	while (arr_token[*index].type == TK_WORD)
	{
		nb_of_consecutive_words++;
		(*index)++;
	}
	node->argv = calloc(nb_of_consecutive_words + 1, sizeof(char*)); // number of consecutive words
	if (!node->argv)
		print_red("calloc");
	*index -= nb_of_consecutive_words;
	curr_token = arr_token[*index];
	int i = 0;
	while (arr_token[*index].type == TK_WORD)
	{
		node->argv[i] = strdup(arr_token[*index].str);
		(*index)++;
		i++;
	}
	node->argv[i] = NULL;
	return (node);
}

//  ██████   ██████   █████████   █████ ██████   █████
// ▒▒██████ ██████   ███▒▒▒▒▒███ ▒▒███ ▒▒██████ ▒▒███ 
//  ▒███▒█████▒███  ▒███    ▒███  ▒███  ▒███▒███ ▒███ 
//  ▒███▒▒███ ▒███  ▒███████████  ▒███  ▒███▒▒███▒███ 
//  ▒███ ▒▒▒  ▒███  ▒███▒▒▒▒▒███  ▒███  ▒███ ▒▒██████ 
//  ▒███      ▒███  ▒███    ▒███  ▒███  ▒███  ▒▒█████ 
//  █████     █████ █████   █████ █████ █████  ▒▒█████
// ▒▒▒▒▒     ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒    ▒▒▒▒▒ 

int main(void)
{
	char *str;
	t_token	*arr_token; 
	t_node *node;

	signal(SIGINT, acknowledge_sig);
	signal(SIGQUIT, SIG_IGN);
	// fork();
	while(1)
	{
		// str = readline("👻>");
		str = readline("ghost>");
		if (!str)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(str))
		{
			add_history(str);
			arr_token = tokenize(str);
			print_tokens(arr_token);
			printf("\n");
			node = parse_tokens(arr_token);
			print_nodes(node, 0);
		}
		free(str);
	}
	return (0);
}
