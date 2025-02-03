/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:23:10 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/03 16:30:09 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"

static void	validate_open_pipe(int fd[2], int check, pid_t pid[2]);
static void	plumbing(int fd[2], int pipefd[2], pid_t pid, char **argv);
static char ***parse_args(char **argv, int argc);
static void free_close_and_print_error(t_pipe *pipe);

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe  pipe;

	if (argc != 5 && argv[1] && argv[2] && argv[3] && argv[4])
		exit(perror("Usage: pipex <file> <cmd> <cmd> <file>\n", EXIT_FAILURE));
	else if (*envp == NULL)
		exit(perror("No enviroment pointer.\n"), EXIT_FAILURE);
	pipe.cmd = parse_args(argv, argc);
	pipe.fd[0] = open(argv[1], O_RDONLY);
	pipe.fd[1] = open(argv[argc - 1], O_WRONLY);
	pipe.check = pipe(pipe.pfd);
	validate_open_pipe_pid_child_cmds(pipe.fd, pipe.check, pipe.pid);
	plumbing(&pipe, &argv[1])
}

static char ***parse_args(char **argv)
{
	char ***cmd;
	int i;
	int i1;

	i = 2;
	i1 = 0;
	cmd = malloc(sizeof(char ***) * (argc - 3));
	if (!cmd)
		return (NULL);
	while(argv[i])
		cmd[i1++] = ft_split(argv[i++], ' ');
}

static void	plumbing(t_pipe *pipe, char **argv)
{
	int i;

	i = 0;
	pipe->fd[0]
	while(i < cmds)
	{
		subprocess(pipe, fork(), false, i++);
		subprocess(pipe, fork(), true, i++);
	}
	close(pipe->pfd[0]);
	close(pipe->pfd[1]);
	while ()
}

static void	validate_open_pipe(int fd[2], int pipe)
{
	if (fd[0] < 0 || fd[1] < 0)
	{
		if (fd[1] > -1)
			close(fd[1]);
		if (fd[0] > -1)
			close(fd[0]);
		perror("open");
		exit(EXIT_FAILURE);
	}
	else if (pipe == -1)
	{
		close(fd[1]);
		close(fd[0]);
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid[0] && pid[1])
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}
