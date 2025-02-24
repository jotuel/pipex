/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:40:24 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/24 22:27:16 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

/*
** frees all heap memory.
*/
void	free_all(t_pipe *pipex)
{
	int	i;
	int	i1;

	i = 0;
	i1 = 0;
	while (pipex->cmd[i1])
	{
		while (pipex->cmd[i1][i])
		{
			free(pipex->cmd[i1][i]);
			pipex->cmd[i1][i++] = NULL;
		}
		free(pipex->cmd[i1]);
		pipex->cmd[i1] = NULL;
		i = 0;
		i1++;
	}
	free(pipex->cmd);
	pipex->cmd = NULL;
	i = 0;
	while (pipex->path[i])
		free(pipex->path[i++]);
	free(pipex->path);
}

/*
** duplicates pipe read end to stdin and file to stdout.
** then closes excess file descriptors and executes command.
** if anything fails it prints error message and exits.
*/
void	dst_subprocess(t_pipe *pipex, char **cmd, char **envp)
{
	if (-1 == dup2(pipex->pfd[0], STDIN_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == dup2(pipex->fd[1], STDOUT_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == close(pipex->pfd[0]))
		free_and_exit(pipex, NULL, "close", errno);
	if (-1 == close(pipex->pfd[1]))
		free_and_exit(pipex, NULL, "close1", errno);
	if (-1 == close(pipex->fd[1]))
		free_and_exit(pipex, NULL, "close", errno);
	pipex->fd[1] = -1;
	if (-1 == close(pipex->fd[0]))
		free_and_exit(pipex, NULL, "close", errno);
	pipex->fd[0] = -1;
	if (-1 == execve(cmd[0], cmd, envp))
		free_and_exit(pipex, cmd[0], cmd[0], errno);
}

/*
** duplicates file to stdin and pipe write end to stdout.
** then closes excess file descriptors and executes command.
** if anything fails it prints error message and exits.
*/
void	src_subprocess(t_pipe *pipex, char **cmd, char **envp)
{
	if (-1 == dup2(pipex->pfd[1], STDOUT_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == dup2(pipex->fd[0], STDIN_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == close(pipex->pfd[1]))
		free_and_exit(pipex, NULL, "close", errno);
	if (-1 == close(pipex->pfd[0]))
		free_and_exit(pipex, NULL, "close", errno);
	if (-1 == close(pipex->fd[0]))
		free_and_exit(pipex, NULL, "close", errno);
	pipex->fd[0] = -1;
	if (-1 == close(pipex->fd[1]))
		free_and_exit(pipex, NULL, "close", errno);
	pipex->fd[1] = -1;
	if (-1 == execve(cmd[0], cmd, envp))
		free_and_exit(pipex, cmd[0], cmd[0], errno);
}

/*
** if its the original process it has subprocessed pid and returns
** with that, otherwise it goes off based on the value of dest flag
** to execute in either src_ or dst_subprocess function.
*/
pid_t	subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth)
{
	if (!pid)
		if (!dest)
			src_subprocess(pipex, pipex->cmd[nth], pipex->envp);
	else
		dst_subprocess(pipex, pipex->cmd[nth], pipex->envp);
	else if (-1 == pid)
		free_and_exit(pipex, NULL, "fork", errno);
	else
		return (pid);
	return (-1);
}

/*
** this join / to end of every file in path and that to command
** then checks with access if file exists and tries to see
** whether that file is also executable (has those rights).
*/
bool	command_in_path(t_pipe *pipex, int nbr, char *cmd_p, int i)
{
	char	*tmp;

	while (pipex->path[i])
	{
		tmp = ft_strjoin(pipex->path[i], "/");
		if (!tmp)
			free_and_exit(pipex, NULL, "malloc", errno);
		cmd_p = ft_strjoin(tmp, pipex->cmd[nbr][0]);
		if (!cmd_p)
			free_and_exit(pipex, NULL, "malloc", errno);
		free(tmp);
		if (0 == access(cmd_p, X_OK | F_OK))
		{
			free(pipex->cmd[nbr][0]);
			pipex->cmd[nbr][0] = cmd_p;
			break ;
		}
		else
			free(cmd_p);
		i++;
	}
	if (!access(pipex->cmd[nbr][0], X_OK | F_OK) && path_is_absolute(pipex,
			nbr))
		return (false);
	return (true);
}
