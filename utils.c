/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:09:01 by jtuomi            #+#    #+#             */
/*   Updated: 2025/02/21 17:13:30 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char **is_path_in_env(t_pipe *pipex, char *s, char *s1, int i)
{
	while(pipex->envp[i])
	{
		s1 = ft_strnstr(pipex->envp[i], s, 5);
		if (s1)
			return (ft_split(&pipex->envp[i][5], ':'));
		i++;
	}
	return (NULL);
}

void	util_parse_args(t_pipe *pipex, char *tmp, int i)
{
	pipex->path = is_path_in_env(pipex, "PATH=", NULL, 0);
	if (!pipex->path)
		free_and_exit(pipex, NULL, "path", errno);
	i = 0;
	while (pipex->cmd[i])
	{
		if (!command_in_path(pipex, i, NULL, 0))
		{
			tmp = ft_strtrim(pipex->cmd[i][0], "/");
			if (!tmp)
				free_and_exit(pipex, NULL, "malloc", errno);
			free(pipex->cmd[i][0]);
			pipex->cmd[i][0] = tmp;
		}
		i++;
	}
}

bool path_is_absolute(t_pipe *pipex, int nbr)
{
	return (ft_strchr(pipex->cmd[nbr][0], '/'));
}

void free_and_exit(t_pipe *pipex, char *cmd, char *err, int error)
{
	if (!error)
		return ;
	if (!cmd)
		perror(err);
	else
		ft_printf("pipex: %s: %s", err, cmd);
	free_all(pipex);
	exit(error);
}
