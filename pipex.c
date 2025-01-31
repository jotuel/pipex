/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:23:10 by jtuomi            #+#    #+#             */
/*   Updated: 2025/01/31 13:08:59 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"

static void	validate_open_pipe_pid(int fd[2], int pipefd, pid_t pid);
static void	plumbing(int fd[2], int pipefd[2], pid_t pid, char **argv);

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		pipefd[2];
	int		check;
	pid_t	pid[2];

	if (argc < 5)
	{
		perror("Usage: pipex <file> <command> ... <command> <file>\n");
		exit(EXIT_FAILURE);
	}
	fd[0] = open(argv[1], O_RDWR);
	fd[1] = open(argv[argc - 1], O_RDWR);
	check = pipe(pipefd);
	pid[0] = fork();
	pid[1] = fork();
	validate_open_pipe_pid(fd, check, pid);
	plumbing(fd, pipefd, pid, &argv[1])
}

t_command	new_command(t_command command)
{
	command = (t_command)
	{
		.path = arr[BUFSIZ], .args = NULL, arg_len = 0, .fd_in = STDIN_FILENO,
			.fd_out = STDIN_FILENO
	}
	return (command);
}

static void	plumbing(int fd[2], int pipefd[2], pid_t pid, char **argv)
{
	// TODO: pipe file 1 to stdout for execve of cmd 1
	// TODO: pipe output of cmd 1 to stdout for cmd 2 to read
	// TODO: read stdin and write to file 2
}

static void	validate_open_pipe_pid(int fd[2], int pipefd, pid_t pid)
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
	else if (pipefd == -1)
	{
		close(fd[1]);
		close(fd[0]);
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}
