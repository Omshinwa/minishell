/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 11:32:43 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 11:05:26 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (access(path, X_OK) == 0)
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

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	find_path = find_env_node(shell->env, "PATH")->value;
	if (!find_path)
		return (NULL);
	split_path = ft_split(find_path, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i])
	{
		path = try_path(split_path[i], cmd);
		if (path)
			return (free_split(split_path), path);
		i++;
	}
	free_split(split_path);
	return (NULL);
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
			// write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			printf("Quit \n");
			// write(STDOUT_FILENO, "Quit\n", 5);
		return (128 + sig);
	}
	return (0);
}
