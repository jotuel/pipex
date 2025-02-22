/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:23:10 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/21 17:20:03 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	validate_open_pipe(int fd[2], int pipe);
static void	plumbing(t_pipe *pipe, int status);
static char	***parse_args(char **argv, int argc);
static bool	is_all_space(int argc, char **argv);

int	main(int argc, char *argv[], char *envp[])
{
	static t_pipe	pipex;

	if (argc != 5 || is_all_space(argc, argv))
		return (ft_putendl_fd("usage: ./pipex <file> <cmd> <cmd> <file>", 2),
			EXIT_FAILURE);
	else if (*envp == NULL)
		return (ft_putendl_fd("pipex: no enviroment", 2), EXIT_FAILURE);
	pipex.envp = envp;
	pipex.fd[0] = open(argv[1], O_RDWR);
	pipex.fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
	validate_open_pipe(pipex.fd, pipe(pipex.pfd));
	pipex.cmd = parse_args(argv, argc);
	if (!pipex.cmd)
		return (perror("malloc"), errno);
	util_parse_args(&pipex, 0);
	plumbing(&pipex, 0);
}

static char	***parse_args(char **argv, int argc)
{
	char	***cmd;
	int		i;
	int		i1;

	i = 2;
	i1 = 0;
	cmd = ft_calloc(sizeof(char ***), argc - 2);
	if (!cmd)
		return (NULL);
	while (argv[i + 1])
		cmd[i1++] = ft_split(argv[i++], ' ');
	return (cmd);
}

static void	plumbing(t_pipe *pipe, int status)
{
	pipe->pid[0] = subprocess(pipe, fork(), false, 0);
	pipe->pid[1] = subprocess(pipe, fork(), true, 1);
	close(pipe->pfd[1]);
	close(pipe->pfd[0]);
	waitpid(pipe->pid[0], &status, 0);
	waitpid(pipe->pid[1], &status, 0);
	close(pipe->fd[0]);
	close(pipe->fd[1]);
	free_all(pipe);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
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

static bool	is_all_space(int argc, char **argv)
{
	int	i;
	int	i1;
	int	count;

	count = 0;
	i = 0;
	i1 = 0;
	while (argv[i])
	{
		while (argv[i][i1])
		{
			if (argv[i][i1] != ' ')
			{
				count += 1;
				break ;
			}
			i1 += 1;
		}
		i += 1;
	}
	return (argc - 1 == count);
}
