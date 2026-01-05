/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 13:24:31 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/05 13:43:09 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_out(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_heredoc(t_redir *redir)
{
	dup2(redir->fd, STDIN_FILENO);
	close(redir->fd);
	return (0);
}

static int	handle_redir_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == TK_REDIR_IN && handle_redir_in(redir))
			return (1);
		else if (redir->type == TK_REDIR_OUT && handle_redir_out(redir))
			return (1);
		else if (redir->type == TK_REDIR_2IN && handle_redir_heredoc(redir))
			return (1);
		else if (redir->type == TK_REDIR_2OUT && handle_redir_append(redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}
