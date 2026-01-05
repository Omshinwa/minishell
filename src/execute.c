//  ██████████ █████ █████ ██████████
// ▒▒███▒▒▒▒▒█▒▒███ ▒▒███ ▒▒███▒▒▒▒▒█
//  ▒███  █ ▒  ▒▒███ ███   ▒███  █ ▒ 
//  ▒██████     ▒▒█████    ▒██████   
//  ▒███▒▒█      ███▒███   ▒███▒▒█   
//  ▒███ ▒   █  ███ ▒▒███  ▒███ ▒   █
//  ██████████ █████ █████ ██████████
// ▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒  

#include "minishell_exec.h"

int	execute_pipeline(t_node *node, t_shell *shell)
{
	(void) node;
	(void) shell;
	return (0);
}

int	execute_subshell(t_node *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0) // child
		return (execute_tree(node->left, shell));
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
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

int	execute_simple_command(t_node *node, t_shell *shell)
{
	char	*path;
	pid_t	pid;
	int		status;

	// 1. TODO: Check for builtins (cd, export, exit, unset) here!
	// printf("EXE:\n");
	// print_nodes(node, 0);
	path = get_path(node->argv[0], shell);
	if (!path)
	{
		write(2, "command not found: ", 19);
		write(2, node->argv[0], ft_strlen(node->argv[0]));
		write(2, "\n", 1);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return (1);
	}
	if (pid == 0) // child
	{
		// do redirections
		t_redir	*tmp;
		tmp = node->redir;
		while (tmp)
		{
			if (tmp->type == TK_REDIR_2IN)
				dup2(tmp->fd, STDIN_FILENO);
			tmp = tmp->next;
		}

		// execute
		if (execve(path, node->argv, shell->envp) == -1)
		{
			print_red("execve");
			free(path);
			exit(1);
		}
		
		// close the fds
		tmp = node->redir;
		while (tmp)
		{
			if (tmp->type == TK_REDIR_2IN)
				close(tmp->fd);
			tmp = tmp->next;
		}
	}
	free(path);
	waitpid(pid, &status, 0);
	// you can delete this idk the AI told me we need WIFEXITED
	// because status isnt just 0 or something.
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	// Case 2: Child was killed by a signal (Ctrl+C, Segfault)
	// not tested yet
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status)); 
	// Standard shell behavior (128 + signal ???)
	return (0);
}


int	execute_tree(t_node *node, t_shell *shell)
{
	int status;

	if (!node)
		return (0);
	if (node->type == NODE_AND)
	{
		status = execute_tree(node->left, shell); // Run left side
		if (status == 0)                   // If success
			return (execute_tree(node->right, shell)); // Run right side
		return (status);                   // Otherwise return failure
	}

	// 2. Handle Logical OR (||)
	if (node->type == NODE_OR)
	{
		status = execute_tree(node->left, shell); // Run left side
		if (status != 0)                   // If failure
			return (execute_tree(node->right, shell)); // Run right side
		return (status);                   // Otherwise return success
	}

	// 3. Handle Pipes (|)
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));

	if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, shell));

	if (node->type == NODE_CMD)
		return (execute_simple_command(node, shell));

	return (0);
}
