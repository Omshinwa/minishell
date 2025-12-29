/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:06:49 by dasamuel          #+#    #+#             */
/*   Updated: 2025/12/19 00:27:40 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	open_files(int *infile, int *outfile, char **av)
{
	*infile = open(av[1], O_RDONLY);
	if (*infile == -1)
		error_exit(av[1]);
	*outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*outfile == -1)
		error_exit(av[4]);
}

void	first_child(int infile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[0]);
	if (dup2(infile, STDIN_FILENO) == -1)
		error_exit("dup2 infile");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 pipe");
	close(pipe_fd[1]);
	close(infile);
	execute_command(cmd, envp);
}

void	second_child(int outfile, int *pipe_fd, char *cmd, char **envp)
{
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("dup2 pipe");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		error_exit("dup2 outfile");
	close(pipe_fd[0]);
	close(outfile);
	execute_command(cmd, envp);
}

void	close_all_fds(int infile, int outfile, int *pipe_fd)
{
	close(infile);
	close(outfile);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

int	main(int ac, char **av, char **envp)
{
	int		infile;
	int		outfile;
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (ac != 5)
		return (write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37), 1);
	open_files(&infile, &outfile, av);
	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("fork");
	if (pid1 == 0)
		first_child(infile, pipe_fd, av[2], envp);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("fork");
	if (pid2 == 0)
		second_child(outfile, pipe_fd, av[3], envp);
	close_all_fds(infile, outfile, pipe_fd);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
