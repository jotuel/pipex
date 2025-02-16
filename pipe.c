/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipe.c                                         __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/02 18:12:35 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/16 19:28:26 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(t_pipe *pipex)
{
	int	i;

	i = 0;
	while (pipex->cmd[0][i])
		free(pipex->cmd[0][i++]);
	free(pipex->cmd[0][i]);
	i = 0;
	while (pipex->cmd[1][i])
		free(pipex->cmd[1][i++]);
	free(pipex->cmd[1][i]);
	i = 0;
	while (pipex->path[i])
		free(pipex->path[i++]);
	free(pipex->path[i]);
}

void	dest_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	if (-1 == dup2(pipex->pfd[1], STDOUT_FILENO))
	{
		perror("dup2");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	if (-1 == dup2(fd, STDIN_FILENO))
	{
		perror("dup2");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	if (-1 == close(pipex->pfd[0]))
	{
		perror("close");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	execve(cmd[0], cmd, envp);
	perror("execve");
	free_all(pipex);
	ft_printf("destination");
	exit(WEXITED);
}

void	src_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	if (-1 == dup2(pipex->pfd[0], STDIN_FILENO))
	{
		perror("dup2");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	if (-1 == dup2(fd, STDOUT_FILENO))
	{
		perror("dup2");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	if (-1 == close(pipex->pfd[1]))
	{
		perror("close");
		free_all(pipex);
		exit(EXIT_FAILURE);
	}
	execve(cmd[0], cmd, envp);
	perror("execve");
	free_all(pipex);
	ft_printf("source");
	exit(errno);
}

pid_t	subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth)
{
	if (!pid)
		if (!dest)
			src_subprocess(pipex, pipex->cmd[nth], pipex->envp, pipex->fd[0]);
		else
			dest_subprocess(pipex, pipex->cmd[nth], pipex->envp, pipex->fd[1]);
	else if (pid == -1)
		perror("fork");
	else
		return (pid);
	free_all(pipex);
	exit(errno);
}

bool	commands_in_path(t_pipe *pipex, int i, int i1, char *cmdp)
{
	while (pipex->path[i])
	{
		cmdp = ft_strjoin(pipex->path[i], pipex->cmd[0][0]);
		if (0 == access(cmdp, X_OK | F_OK))
		{
			free(pipex->cmd[0][0]);
			pipex->cmd[0][0] = cmdp;
			break ;
		}
		else
			free(cmdp);
		i++;
	}
	while (pipex->path[i1])
	{
		cmdp = ft_strjoin(pipex->path[i1], pipex->cmd[1][0]);
		if (0 == access(cmdp, X_OK | F_OK))
		{
			free(pipex->cmd[1][0]);
			pipex->cmd[1][0] = cmdp;
			break ;
		}
		else
			free(cmdp);
		i1++;
	}
	if (access(pipex->cmd[0][0], X_OK) && access(pipex->cmd[1][0], X_OK))
		return (false);
	return (true);
}
