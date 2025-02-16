/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipe.c                                         __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/02 18:12:35 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/16 15:54:02 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(t_pipe *pipex)
{
	int i;

	i = 0;
	while(pipex->cmd[0][i])
		free(pipex->cmd[0][i++]);
	free(pipex->cmd[0][i]);
	i = 0;
	while(pipex->cmd[1][i])
		free(pipex->cmd[1][i++]);
	free(pipex->cmd[1][i]);
	i = 0;
	while(pipex->path[i])
		free(pipex->path[i++]);
	free(pipex->path[i]);
}

void	dest_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	ft_printf("%s %s\n", cmd[0], cmd[1]);
	dup2(pipex->pfd[1], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(pipex->pfd[0]);
	close(pipex->pfd[1]);
//	free_all(pipex);
	if (-1 == execve(cmd[0], cmd, envp))
		perror("execve");
	free_all(pipex);
	exit(errno);
}

void	src_subprocess(t_pipe *pipex, char **cmd, char **envp, int fd)
{
	ft_printf("%s %s\n", cmd[0], cmd[1]);
	dup2(pipex->pfd[0], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	close(pipex->pfd[1]);
	close(pipex->pfd[0]);
//	free_all(pipex);
	if (-1 == execve(cmd[0], cmd, envp))
		perror("execve");
	free_all(pipex);
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
	exit(errno);
}

bool commands_in_path(t_pipe *pipex, int i, int i1, char *cmdp)
{
	while(pipex->path[i])
	{
		cmdp = ft_strjoin(pipex->path[i], pipex->cmd[0][0]);
		if (0 == access(cmdp, F_OK))
		{
			free(pipex->cmd[0][0]);
			pipex->cmd[0][0] = cmdp;
			break;
		}
		else
			free(cmdp);
		i++;
	}
	while(pipex->path[i1])
	{
		cmdp = ft_strjoin(pipex->path[i1], pipex->cmd[1][0]);
		if (0 == access(cmdp, F_OK))
		{
			free(pipex->cmd[1][0]);
			pipex->cmd[1][0] = cmdp;
			break;
		}
		else
			free(cmdp);
		i1++;
	}
	if (access(pipex->cmd[0][0], X_OK) && access(pipex->cmd[1][0], X_OK))
		return (false);
	return (true);
}
