/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:23:10 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/24 19:21:43 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"

static void	plumbing(t_pipe *pipe, int status);
static char	***parse_args(char **argv, int argc, t_pipe *pipe);
static bool	not_all_space(char **argv);

/*
** pipex takes two commands and two files and pipes input file -> first cmd
** -> second cmd -> output file. it fails if input file doesn't exist, either
** is a directory or no path is set. otherwise it forks and executes cmd's
** on subprocesses
 */
int	main(int argc, char *argv[], char *envp[])
{
	static t_pipe	pipex;

	if ((argc == 5 && not_all_space(argv)))
		return (ft_putendl_fd("usage: ./pipex <file> <cmd> <cmd> <file>", 2),
			EXIT_FAILURE);
	else if (*envp == NULL)
		return (ft_putendl_fd("pipex: no enviroment", 2), EXIT_FAILURE);
	pipex.envp = envp;
	pipex.fd[0] = open(argv[1], O_RDWR);
	if (-1 == pipex.fd[0])
		free_and_exit(&pipex, argv[1], strerror(errno), EXIT_FAILURE);
	pipex.fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (-1 == pipex.fd[1])
		free_and_exit(&pipex, argv[argc - 1], strerror(errno), EXIT_FAILURE);
	if (-1 == pipe(pipex.pfd))
		free_and_exit(&pipex, "pipe", strerror(errno), EXIT_FAILURE);
	pipex.cmd = parse_args(argv, argc, &pipex);
	if (!pipex.cmd)
		return (perror("malloc"), errno);
	util_parse_args(&pipex, 0);
	plumbing(&pipex, 0);
}

/*
** this function allocates space for pointers. splits the commands
** stores them in the array and returns them to caller.
**
** if anything allocation fails it exits, freeing all.
 */
static char	***parse_args(char **argv, int argc, t_pipe *pipe)
{
	char	***cmd;
	int		i;
	int		i1;

	i = 2;
	i1 = 0;
	cmd = ft_calloc(sizeof(char ***), argc - 2);
	if (!cmd)
		free_and_exit(pipe, NULL, "malloc", errno);
	while (argv[i + 1])
	{
		cmd[i1] = ft_split(argv[i++], ' ');
		if (!cmd[i1++])
			free_and_exit(pipe, NULL, "malloc", errno);
	}

	return (cmd);
}

/*
** creates forks, closes pipe file descriptors and waits for sub-
** processes to finish. if both terminate returns the error value
** of the last one or goes back to main (returning 0)
 */
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

/*
** checks if all characters are ' ', the character used to split.
 */
static bool	not_all_space(char **argv)
{
	int	i;
	int	i1;
	bool flag;

	i = 0;
	i1 = 0;
	while (argv[i])
	{
		flag = true;
		while (argv[i][i1])
		{
			if (argv[i][i1] != '\\' && argv[i][i1] != ' ')
			{
				flag = false;
				break ;
			}
			i1 += 1;
		}
		if (flag == true)
			break;
		i += 1;
	}
	return (flag);
}
