/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   pipex.h                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/01/31 12:59:37 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/02 18:21:06 by jtuomi           \__/    i               */
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

typedef struct s_pipe
{
	char ***cmd;
	int fd[2];
	int pfd[2];
	int check;
	pid_t pid[2];
} t_pipe;

int			open_pipex(const char *pipex_path);
void		close_pipex(int fd);
int			read_pipex(int fd, void *buf, size_t count);
int			write_pipex(int fd, const void *buf, size_t count);
#endif
