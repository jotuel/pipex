/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipex.h                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/01/31 12:59:37 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/06 20:02:49 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include "libft/libft.h"

typedef struct s_pipe
{
	char ***cmd;
	char **envp;
	char **path;
	int fd[2];
	int pfd[2];
	int check;
	pid_t pid[2];
} t_pipe;

bool        commands_in_path(t_pipe *pipex, int i, char *cmdp, char *cmdp1);
void		src_subprocess(t_pipe *pipex, char **cmd, char **envp);
void		dest_subprocess(t_pipe *pipex, char **cmd, char **envp);
void        free_all(t_pipe *pipex);
pid_t		subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth);
#endif
