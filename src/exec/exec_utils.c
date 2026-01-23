/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 11:32:43 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/14 06:46:27 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	file_exists(char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0);
}

void	free_split(char **split)
{
	size_t	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*try_path(char *dir, char *cmd)
{
	char	*temp;
	char	*path;

	temp = ft_strjoin(dir, "/");
	path = ft_strjoin(temp, cmd);
	free(temp);
	if (file_exists(path))
		return (path);
	free(path);
	return (NULL);
}

char	*get_path(char *cmd, t_shell *shell)
{
	int		i;
	char	*find_path;
	char	**split_path;
	char	*path;
	t_env	*path_node;

	if (ft_is_in_set(cmd, '/') && file_exists(cmd))
		return (ft_strdup(cmd));
	else if (ft_is_in_set(cmd, '/') && !file_exists(cmd))
		return (NULL);
	path_node = find_env_node(shell->env, "PATH");
	if (!path_node || !path_node->value)
		return (NULL);
	find_path = path_node->value;
	split_path = ft_split(find_path, ':');
	if (!split_path)
		return (NULL);
	i = -1;
	while (split_path[++i])
	{
		path = try_path(split_path[i], cmd);
		if (path)
			return (free_split(split_path), path);
	}
	return (free_split(split_path), NULL);
}

int	get_exit_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		printf(" Terminated by signal %i. ", sig);
		if (sig == SIGINT)
			printf("\n");
		else if (sig == SIGQUIT)
			printf("Quit \n");
		return (128 + sig);
	}
	return (0);
}
