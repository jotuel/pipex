/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:23:10 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/06 20:11:51 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"

extern char const	**environ;

static void			validate_open_pipe(int fd[2], int pipe);
static void			plumbing(t_pipe *pipe, int cmds);
static char			***parse_args(char **argv, int argc);

int	main(int argc, char *argv[], char *envp[])
{
	static t_pipe	pipex;

	if (argc != 5 && argv[1] && argv[2] && argv[3] && argv[4])
		return (perror("Usage: pipex <file> <cmd> <cmd> <file>\n"),
			EXIT_FAILURE);
	else if (*envp == NULL)
		return (perror("No enviroment pointer.\n"), EXIT_FAILURE);
	pipex.envp = envp;
	pipex.cmd = parse_args(argv, argc);
	if (!pipex.cmd)
		return (perror("malloc"), EXIT_FAILURE);
	pipex.fd[0] = open(argv[1], O_RDONLY);
	pipex.fd[1] = open(argv[argc - 1], O_CREAT, 0644, O_WRONLY);
	pipex.check = pipe(&pipex.pfd[2]);
	validate_open_pipe(pipex.fd, pipex.check);
	commands_in_path(&pipex, 0, NULL, NULL);
	plumbing(&pipex, argc - 3);
}

static char	***parse_args(char **argv, int argc)
{
	char	***cmd;
	int		i;
	int		i1;

	i = 2;
	i1 = 0;
	cmd = malloc(sizeof(char ***) * (argc - 2));
	if (!cmd)
		return (NULL);
	while (argv[i])
		cmd[i1++] = ft_split(argv[i++], ' ');
	return (cmd);
}

static void	plumbing(t_pipe *pipe, int cmds)
{
	int	i;
	int	stat;

	i = 0;
	dup2(pipe->fd[0], STDIN_FILENO);
	close(pipe->fd[0]);
	while (i < cmds)
	{
		pipe->pid[0] = subprocess(pipe, fork(), false, i++);
		pipe->pid[1] = subprocess(pipe, fork(), true, i++);
	}
	dup2(pipe->fd[1], STDOUT_FILENO);
	close(pipe->fd[1]);
	close(pipe->pfd[0]);
	close(pipe->pfd[1]);
	waitpid(pipe->pid[0], &stat, WNOHANG);
	free_close_and_print_error(pipe, false, 0);
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
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}
