/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 13:25:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/02 21:02:55 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parse.h"

//   █████████  █████   █████████  ██████   █████   █████████   █████      
//  ███▒▒▒▒▒███▒▒███   ███▒▒▒▒▒███▒▒██████ ▒▒███   ███▒▒▒▒▒███ ▒▒███       
// ▒███    ▒▒▒  ▒███  ███     ▒▒▒  ▒███▒███ ▒███  ▒███    ▒███  ▒███       
// ▒▒█████████  ▒███ ▒███          ▒███▒▒███▒███  ▒███████████  ▒███       
//  ▒▒▒▒▒▒▒▒███ ▒███ ▒███    █████ ▒███ ▒▒██████  ▒███▒▒▒▒▒███  ▒███       
//  ███    ▒███ ▒███ ▒▒███  ▒▒███  ▒███  ▒▒█████  ▒███    ▒███  ▒███      █
// ▒▒█████████  █████ ▒▒█████████  █████  ▒▒█████ █████   █████ ███████████
//  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒    ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒▒ 

volatile sig_atomic_t	g_ackowledge = 0;
// an atomic operation guarantees that every read/write from g_acknoledge are
// done in full before handling a signal.
// volatile prevents the compiler from making some optimizations

void	acknowledge_sig(int signum)
{
	g_ackowledge = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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

void	clean_tokens(t_token **arr_token)
{
	int	i;

	i = 0;
	while((*arr_token)[i].type != TK_END)
	{
		free((*arr_token)[i].str);
		i++;
	}
	free(*arr_token);
	*arr_token = NULL;
}

// give the first char of a new token, 
// change the token type
// return i as the progress made
int	set_token(t_token *token, char *str, int start)
{
	// TK_WORD
	int	end; // return
	int	c;

	end = start;
	c = str[start];
	token->str = NULL;
	if (c == '!')
		return (token->type = TK_END, printf( RED "invalid char: %c", c), -1);
	else if (!is_metachar(c) || c == '"' ||  c == '\'') // until space
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
					return (token->type = TK_END, print_red("unclosed quotation"), -1);
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
			return (token->type = TK_END, print_red("invalid char: &"), -1);
	}
	// TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_2IN, TK_REDIR_2OUT
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
		return (token->type = TK_END, print_red("invalid char"),-1);
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
	arr_token = safe_malloc(sizeof(t_token) * (ft_strlen(str) + 1));
	if (!arr_token)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ')
		{
			i++;
			continue;
		}
		arr_token[j] = (t_token) {NULL, TK_END};
		new_i = set_token(&arr_token[j], str, i);
		if (new_i == -1)
		{
			printf(RED "invalid token at index %i : %c" RESET "\n", i, str[i]);
			break ;
		}
		i = new_i;
		j++;
	}
	arr_token[j] = (t_token) {NULL, TK_END};
	if (new_i == -1)
		clean_tokens(&arr_token);
	return (arr_token);
}

void print_tokens(t_token *arr_token)
{
	int	i;
	t_token	token;

	i = -1;
	if (!arr_token)
		return;
	while(arr_token[++i].type != TK_END)
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

//  ███████████    █████████   ███████████    █████████  ██████████
// ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███ ▒▒███▒▒▒▒▒███  ███▒▒▒▒▒███▒▒███▒▒▒▒▒█
//  ▒███    ▒███ ▒███    ▒███  ▒███    ▒███ ▒███    ▒▒▒  ▒███  █ ▒ 
//  ▒██████████  ▒███████████  ▒██████████  ▒▒█████████  ▒██████   
//  ▒███▒▒▒▒▒▒   ▒███▒▒▒▒▒███  ▒███▒▒▒▒▒███  ▒▒▒▒▒▒▒▒███ ▒███▒▒█   
//  ▒███         ▒███    ▒███  ▒███    ▒███  ███    ▒███ ▒███ ▒   █
//  █████        █████   █████ █████   █████▒▒█████████  ██████████
// ▒▒▒▒▒        ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒▒▒▒▒▒ 

//					parse_expression
//						↓
//					parse_pipeline
//						↓
//					parse_command
//					↓			↘
//	parse_simple_command	parse_expression (only for '(')

// Create a new node in the AST
t_node *new_node(t_node_type type, t_node *left, t_node *right)
{
	t_node *node = safe_malloc(sizeof(t_node));

	if (!node)
		return (clean_nodes(&left), clean_nodes(&right), NULL);
	node->type = type;
	node->argv = NULL;	// only for CMD nodes
	node->redir = NULL;	// only for CMD nodes
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

void	clean_nodes(t_node **node_addr)
{
	int	i;
	t_redir	*redir;
	t_redir	*redir_next;
	t_node	*node;

	if (node_addr == NULL)
		return ;
	node = *node_addr;
	if (node == NULL)
		return ;
	clean_nodes(&node->left);
	clean_nodes(&node->right);
	i = 0;
	while(node->argv && node->argv[i])
		free(node->argv[i++]);
	free(node->argv);
	redir = node->redir;
	while (redir)
	{
		free(redir->filename);
		redir_next = redir->next;
		free(redir);
		redir = redir_next;
	}
	free(node);
	*node_addr = NULL;
}

// When the token at ARR_TOKEN[*INDEX] is < or > or << or >>:
// Sets / append the NODE->REDIR
void	parse_redirection(t_node *node,	t_token *arr_token, int *index)
{
	t_redir *redir;
	t_redir *tmp;

	redir = safe_malloc(sizeof(t_redir));
	if (!redir)
	{
		(*index)++;
		return ;
	}
	redir->type = arr_token[*index].type;
	(*index)++;
	redir->filename = ft_strdup(arr_token[*index].str);
	redir->next = NULL;
	(*index)++;
	// add it to the next
	tmp = node->redir;
	if (!tmp)
	{
        node->redir = redir;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = redir;
	return ;
}

t_node	*parse_simple_command(t_token *arr_token, int *index);
t_node *parse_expression(t_token *arr_token, int *index);
t_node *parse_pipeline(t_token *arr_token, int *index);
t_node *parse_command(t_token *arr_token, int *index);

t_node *parse_expression(t_token *arr_token, int *index)
{
	t_node *node;
	t_token_type	op;

	node = parse_pipeline(arr_token, index);
	while (node && arr_token[*index].type & (TK_OP_AND | TK_OP_OR))
	{
		op = arr_token[*index].type;
		(*index)++;
		node = new_node(token_to_node_type(op),
			node, parse_pipeline(arr_token, index));
		if (!node)
			return (NULL);
		if (node->left == NULL || node->right == NULL)
			clean_nodes(&node);
	}
	return (node);
}

// Should never have index pointing at an && or || token.
t_node *parse_pipeline(t_token *arr_token, int *index)
{
	t_node *node;

	if (arr_token[*index].type == TK_OP_AND ||
		arr_token[*index].type == TK_OP_OR)
		return (printf(RED "invalid logic operator `%s`" RESET "\n", arr_token[*index].str), NULL);
	node = parse_command(arr_token, index);
	while (node && arr_token[*index].type == TK_PIPE)
	{
		(*index)++;
		node = new_node(NODE_PIPE, node, parse_command(arr_token, index));
		if (!node)
			return (NULL);
		if (node->left == NULL || node->right == NULL)
			clean_nodes(&node);
	}
	return node;
}

t_node *new_subshell(t_node *node)
{
	if (node == NULL)
		return (node);
	node = new_node(NODE_SUBSHELL, node, NULL);
	return (node);
}

// Should never have index pointing at an &&, ||, or pipe token.
t_node *parse_command(t_token *arr_token, int *index)
{
	t_node *node;
	t_token curr_token;

	if (arr_token[*index].type == TK_OP_AND ||
		arr_token[*index].type == TK_OP_OR  ||
		arr_token[*index].type == TK_PIPE)
		return (printf(RED "416: syntax error near unexpected token `%s`" RESET "\n", arr_token[*index].str), NULL);
	curr_token = arr_token[*index];
	if (curr_token.type == TK_OPEN_PARENT)
	{
		(*index)++;
		node = parse_expression(arr_token, index);
		if (arr_token[*index].type == TK_CLOSE_PARENT)
			return ((*index)++, new_subshell(node));
		else
			return(print_red("missing closing parenthesis."), NULL);
	}
	node = parse_simple_command(arr_token, index);
	return (node);
}

// Should never have index pointing at an &&, ||, pipe or () token.
t_node	*parse_simple_command(t_token *arr_token, int *index)
{
	t_node *node;
	int	nb_of_consecutive_words;
	int i;

	if (arr_token[*index].type == TK_OP_AND ||
		arr_token[*index].type == TK_OP_OR  ||
		arr_token[*index].type == TK_PIPE   ||
		arr_token[*index].type == TK_OPEN_PARENT ||
		arr_token[*index].type == TK_CLOSE_PARENT)
		return(printf(RED "443: syntax error near unexpected token `%s`" RESET "\n", arr_token[*index].str), NULL);
	node = new_node(NODE_CMD, NULL, NULL);
	nb_of_consecutive_words = 0;
	i = *index;
	while (arr_token[i].type & (TK_WORD | TK_OPEN_PARENT | TK_REDIR_IN | TK_REDIR_OUT | TK_REDIR_2IN | TK_REDIR_2OUT))
	{
		if (arr_token[i].type == TK_WORD)
			nb_of_consecutive_words++;
		else if (arr_token[i].type == TK_OPEN_PARENT)
		{
			print_red("syntax, parenthesis cannot be placed there.");
			return (clean_nodes(&node), NULL);
		}
		else
		{
			i++; // Skip operator (>, <, etc)
			if (arr_token[i].type != TK_WORD)
				{
					print_red("redirection missing file.");
					printf(RED"expected a filename, got `%s` instead." RESET "\n", arr_token[i].str);
					return (clean_nodes(&node), NULL);
				}
		}
		i++;
	}
	node->argv = safe_malloc((nb_of_consecutive_words + 1) * sizeof(char*)); // number of consecutive words
	if (!node->argv)
		return (clean_nodes(&node), NULL);
	i = 0;
	while (arr_token[*index].type & (TK_WORD | TK_REDIR_IN | TK_REDIR_OUT | TK_REDIR_2IN | TK_REDIR_2OUT))
	{
		if (arr_token[*index].type == TK_WORD)
		{
			node->argv[i++] = ft_strdup(arr_token[*index].str);
			(*index)++;
		}
		else
			parse_redirection(node, arr_token, index);
	}
	node->argv[i] = NULL;
	return (node);
}

void	print_nodes(t_node *node, int depth)
{
	char *color;
	int	i;
	char	offset[depth * 7 + 1];
	t_redir *tmp_redir;

	if (!node)
		return ;
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
				printf("%s%s*%13s" RESET "\n", offset, color, node->argv[i]);
			else
				printf("%s%s%14s" RESET "\n", offset, color, node->argv[i]);
			i++;
		}
		tmp_redir = node->redir;
		while (tmp_redir != NULL)
		{
			color = BG_MAGENTA;
			if (tmp_redir->type == TK_REDIR_IN)
				printf("%s%s<%13s" RESET "\n", offset, color, tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_OUT)
				printf("%s%s>%13s" RESET "\n", offset, color, tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_2IN)
				printf("%s%s<<%12s" RESET "\n", offset, color, tmp_redir->filename);
			if (tmp_redir->type == TK_REDIR_2OUT)
				printf("%s%s>>%12s" RESET "\n", offset, color, tmp_redir->filename);
			tmp_redir = tmp_redir->next;
			color = BG_WHITE BOLD;
			i++;
		}
	}
	else if (node->type == NODE_PIPE)
	{
		color = BG_RED;
		printf("%s%s*PIPE: %7s" RESET "\n", offset, color, "|");
	}
	else if (node->type == NODE_AND)
	{
		color = BG_GREEN;
		printf("%s%s*AND: %8s" RESET "\n", offset, color, "&&");
	}
	else if (node->type == NODE_OR)
	{
		color = BG_YELLOW;
		printf("%s%s*OR: %9s" RESET "\n", offset, color, "||");
	}
	else if (node->type == NODE_SUBSHELL)
	{
		color = BG_BLUE;
		printf("%s%s*%13s" RESET "\n", offset, color, "SUBSHELL     ");
	}
	while (i++ < 5)
		printf("%s%s %13s" RESET "\n", offset, color, " ");
	if (node->left)
		print_nodes(node->left, depth + 1);
	if (node->right)
		print_nodes(node->right, depth + 1);
}


//    █████████   █████      █████████  ███████████                            
//   ███▒▒▒▒▒███ ▒▒███      ███▒▒▒▒▒███▒█▒▒▒███▒▒▒█                            
//  ▒███    ▒███  ▒███████ ▒███    ▒▒▒ ▒   ▒███  ▒  ████████   ██████   ██████ 
//  ▒███████████  ▒███▒▒███▒▒█████████     ▒███    ▒▒███▒▒███ ███▒▒███ ███▒▒███
//  ▒███▒▒▒▒▒███  ▒███ ▒███ ▒▒▒▒▒▒▒▒███    ▒███     ▒███ ▒▒▒ ▒███████ ▒███████ 
//  ▒███    ▒███  ▒███ ▒███ ███    ▒███    ▒███     ▒███     ▒███▒▒▒  ▒███▒▒▒  
//  █████   █████ ████████ ▒▒█████████     █████    █████    ▒▒██████ ▒▒██████ 
// ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒    ▒▒▒▒▒      ▒▒▒▒▒▒   ▒▒▒▒▒▒  

t_node	*get_abstract_syntax_tree(char *str, int show_debug)
{
	t_token	*arr_token;
	t_node *node;
	int	i;

	arr_token = tokenize(str);
	if (show_debug)
		print_tokens(arr_token);
	i = 0;
	if (!arr_token)
		return (NULL);
	node = parse_expression(arr_token, &i);
	clean_tokens(&arr_token);
	if (show_debug)
		print_nodes(node, 0);
	return (node);
}
