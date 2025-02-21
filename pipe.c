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

void	free_all(t_pipe *pipex)
{
	int	i;
	int i1;

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

void	dst_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	ft_printf("%s %s\n", pipex->cmd[0][0], pipex->cmd[0][1]);
	if (-1 == dup2(pipex->pfd[1], STDOUT_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == dup2(fd, STDIN_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == close(pipex->pfd[0]))
		free_and_exit(pipex, NULL, "close", errno);
	if (-1 == execve(cmd[0], cmd, envp))
		free_and_exit(pipex, NULL, "execve", errno);
}

void	src_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	ft_printf("%s %s\n", pipex->cmd[1][0], pipex->cmd[1][1]);
	if (-1 == dup2(pipex->pfd[0], STDIN_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == dup2(fd, STDOUT_FILENO))
		free_and_exit(pipex, NULL, "dup2", errno);
	if (-1 == close(pipex->pfd[1]))
		free_and_exit(pipex, NULL, "close", errno);
	if (-1 == execve(cmd[1], cmd, envp))
		free_and_exit(pipex, NULL, "execve", errno);
}

pid_t	subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth)
{
	if (!pid)
		if (!dest)
			src_subprocess(pipex, pipex->cmd[nth], pipex->envp, pipex->fd[0]);
		else
			dst_subprocess(pipex, pipex->cmd[nth], pipex->envp, pipex->fd[1]);
	else if (-1 == pid)
		free_and_exit(pipex, NULL, "fork", errno);
	else
		return (pid);
	return (-1);
}

bool	command_in_path(t_pipe *pipex, int nbr, char *cmdp, int i)
{
	char *tmp;

	if (path_is_absolute(pipex, nbr))
		return (false);
	while (pipex->path[i])
	{
		tmp = ft_strjoin(pipex->path[i], "/");
		cmdp = ft_strjoin(tmp, pipex->cmd[nbr][0]);
		if (0 == access(cmdp, X_OK | F_OK))
		{
			free(pipex->cmd[nbr][0]);
			pipex->cmd[nbr][0] = cmdp;
			break ;
		}
		else
			free(cmdp);
		i++;
	}
	if (access(pipex->cmd[nbr][0], X_OK | F_OK))
		return (false);
	return (true);
}
