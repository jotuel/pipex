/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipe.c                                         __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/02 18:12:35 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/06 20:46:33 by jtuomi           \__/    i               */
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

void	dest_subprocess(t_pipe *pipex, char **cmd, char **envp)
{
	dup2(pipex->pfd[0], STDIN_FILENO);
	close(pipex->pfd[0]);
	free_all(pipex);
	if (-1 == execve(cmd[0], cmd, envp))
		perror("execve");
	exit(errno);
}

void	src_subprocess(t_pipe *pipex, char **cmd, char **envp)
{
	dup2(pipex->pfd[1], STDOUT_FILENO);
	close(pipex->pfd[1]);
	free_all(pipex);
	if (-1 == execve(cmd[0], cmd, envp))
		perror("execve");
	exit(errno);
}

pid_t	subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth)
{
	if (!pid)
		if (!dest)
			src_subprocess(pipex, pipex->cmd[nth], pipex->envp);
		else
			dest_subprocess(pipex, pipex->cmd[nth], pipex->envp);
	else if (pid == -1)
		perror("fork");
	else
		return (pid);
	exit(errno);
}
bool commands_in_path(t_pipe *pipex, int i, char *cmdp, char *cmdp1)
{
	util_parse_args(pipex);
	while(pipex->path[i])
	{
		cmdp = ft_strjoin(pipex->path[i], pipex->cmd[0][0]);
		cmdp1 = ft_strjoin(pipex->path[i], pipex->cmd[1][0]);
		if (!access(cmdp, X_OK))
		{
			free(pipex->cmd[0][0]);
			pipex->cmd[0][0] = cmdp;
		}
		else
			free(cmdp);
		if (!access(cmdp1, X_OK))
		{
			free(pipex->cmd[1][0]);
			pipex->cmd[1][0] = cmdp1;
		}
		else
			free(cmdp1);
	}
	if (!(access(pipex->cmd[0][0], X_OK) && access(pipex->cmd[1][0], X_OK)))
		return (true);
	return (false);
}
