/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipe.c                                         __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/02 18:12:35 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/03 16:30:09 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void dest_subprocess(t_pipe *pipe, char **cmd, char **envp)
{
    dup2(pipe->pfd[0], STDIN_FILENO);
    close(pipe->pfd[1]);
    free_all(pipe);
    execve(cmd[0], cmd, envp);
    perror(cmd);
}

void src_subprocess(t_pipe *pipe, char **cmd, char **envp)
{
    dup2(pipe->pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    free_all(pipe);
    execve(cmd[0], cmd, envp);
    perror(cmd);
}

pid_t subprocess(t_pipe *pipe, pid_t pid, bool dest, int nth)
{
    if (!pid)
        if (!dest)
            src_subprocess(pipe, pipe->cmd[nth], pipe->envp);
        else
            dest_subprocess(pipe, pipe->cmd[nth], pipe->envp);
    else if (pid == -1)
        exit(perror(strerror(errno)), EXIT_FAILURE);
    return (pid);
}
