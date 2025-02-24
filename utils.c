/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:09:01 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/24 19:28:03 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** searches env for PATH= and if found splits it. if path would be set as only
** : signs might lead to segfault. this is an edge case not covered as that
** wouldn't be a very good thing to do.
 */
static char	**is_path_in_env(t_pipe *pipex, char *s, char *s1, int i)
{
	while (pipex->envp[i])
	{
		s1 = ft_strnstr(pipex->envp[i], s, 5);
		if (s1)
			return (ft_split(&pipex->envp[i][5], ':'));
		i++;
	}
	return (NULL);
}

/*
** checks whether command can be found and if it can't replaced the command
** later to be executed by /usr/lib/command-not-found to offer suggestions
** how to install or fix a typo etc.
 */
static void	command_not_found(t_pipe *pipe, int nbr)
{
	char	**tmp;
	char	*s;
	int		i;

	tmp = pipe->cmd[nbr];
	s = ft_strjoin("/usr/lib/command-not-found ", pipe->cmd[nbr][0]);
	if (!s)
		free_and_exit(pipe, NULL, "malloc", errno);
	pipe->cmd[nbr] = ft_split(s, ' ');
	free(s);
	i = 0;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}

/*
** this seeks path, check what kind of command we're dealing with and
** calls the other functions accordingly.
 */
void	util_parse_args(t_pipe *pipex, int i)
{
	pipex->path = is_path_in_env(pipex, "PATH=", NULL, 0);
	if (!pipex->path)
		free_and_exit(pipex, NULL, "path", errno);
	i = 0;
	while (pipex->cmd[i])
	{
		if (path_is_absolute(pipex, i))
			if (!access(pipex->cmd[i][0], F_OK | X_OK))
				command_not_found(pipex, i);
			else
				;
		else if (command_in_path(pipex, i, NULL, 0))
			command_not_found(pipex, i);
		i++;
	}
}

/*
** if there is a / in any command it is just executed like in bash.
** checks just for that case.
*/
bool	path_is_absolute(t_pipe *pipex, int nbr)
{
	return (ft_strchr(pipex->cmd[nbr][0], '/'));
}

/*
** this functions closes still open fildes and according to parameters
** prints appropriate error message. then calls free_all before exiting.
*/
void	free_and_exit(t_pipe *pipex, char *cmd, char *err, int error)
{
	if (-1 != pipex->fd[0])
		close(pipex->fd[0]);
	if (-1 != pipex->fd[1])
		close(pipex->fd[1]);
	if (!error)
		return ;
	else if (error == 2)
		error = 127;
	else if (error == 13)
		error = 126;
	if (!cmd)
		perror(err);
	else
	{
		ft_putstr_fd("pipex: ", 2);
		perror(cmd);
	}
	if (pipex->cmd || pipex->path)
		free_all(pipex);
	exit(error);
}
