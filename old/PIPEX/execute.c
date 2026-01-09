/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:13:53 by dasamuel          #+#    #+#             */
/*   Updated: 2026/01/09 10:07:34 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T'
			&& envp[i][3] == 'H' && envp[i][4] == '=')
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
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

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*find_path;
	char	**split_path;
	char	*path;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	find_path = find_path_env(envp);
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

void	execute_command(char *cmd, char **envp)
{
	char	*path;
	char	**cmd_args;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		write(2, "Error: Invalid command\n", 23);
		exit (127);
	}
	path = get_path(cmd_args[0], envp);
	if (!path)
	{
		write(2, "command not found: ", 19);
		write(2, cmd_args[0], ft_strlen(cmd_args[0]));
		write(2, "\n", 1);
		free_split(cmd_args);
		exit(127);
	}
	if (execve(path, cmd_args, envp) == -1)
	{
		free(path);
		free_split(cmd_args);
		error_exit("execve");
	}
}
