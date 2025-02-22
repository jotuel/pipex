/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:40:24 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/21 17:46:39 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

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
	if (-1 == execve(cmd[0], cmd, envp))
		free_and_exit(pipex, NULL, cmd[0], errno);
}

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
	if (-1 == execve(cmd[0], cmd, envp))
		free_and_exit(pipex, NULL, cmd[0], errno);
}

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
	if (access(pipex->cmd[nbr][0], X_OK | F_OK))
		return (false);
	return (true);
}
