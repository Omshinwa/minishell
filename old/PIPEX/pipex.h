/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:09:25 by dasamuel          #+#    #+#             */
/*   Updated: 2025/12/31 17:47:09 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TK_PIPEX_H
# define TK_PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>

char	*find_path_env(char **envp);
char	*try_path(char *dir, char *cmd);
char	*get_path(char *cmd, char **envp);
void	execute_command(char *cmd, char **envp);

void	open_files(int *infile, int *outfile, char **av);
void	first_child(int infile, int *pipe_fd, char *cmd, char **envp);
void	second_child(int outfile, int *pipe_fd, char *cmd, char **envp);
void	close_all_fds(int infile, int outfile, int *pipe_fd);

size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
void	free_split(char **split);

void	error_exit(char *msg);

#endif
