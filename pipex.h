/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipex.h                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/01/31 12:59:37 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/16 15:49:33 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

#define WRONG_ARGUMENTS "Usage: pipex <file> <cmd> <cmd> <file>"

typedef struct s_pipe
{
	char	***cmd;
	char	**envp;
	char	**path;
	int		fd[2];
	int		pfd[2];
	int		check;
	pid_t	pid[2];
}			t_pipe;

bool		path_is_absolute(t_pipe *pipex, int nbr);
bool		command_in_path(t_pipe *pipex, int nbr, char *cmdp, int i);
void		util_parse_args(t_pipe *pipex, int i);
void		src_subprocess(t_pipe *pipex, char **cmd, char **envp);
void		dst_subprocess(t_pipe *pipex, char **cmd, char **envp);
void		free_all(t_pipe *pipex);
void		free_and_exit(t_pipe *pipex, char *cmd, char *err, int error);
pid_t		subprocess(t_pipe *pipex, pid_t pid, bool dest, int nth);

#endif
