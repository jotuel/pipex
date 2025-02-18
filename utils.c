/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   utils.c                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/06 20:27:14 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/18 21:12:34 by jtuomi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"

void	util_parse_args(t_pipe *pipex, char *tmp, int i)
{
	while (pipex->envp[i])
		if (ft_strnstr(pipex->envp[i++], "PATH=", 5))
			break ;
	pipex->path = ft_split(&pipex->envp[i - 1][5], ':');
	if (!pipex->path)
		free_all_and_exit(pipex, "malloc");
	i = 0;
	while(pipex->cmd[i])
	{
		tmp = ft_strjoin("/", pipex->cmd[i][0]);
		if (!tmp)
			free_all_and_exit(pipex, "malloc");
		free(pipex->cmd[i][0]);
		pipex->cmd[i][0] = tmp;
		if (!commands_in_path(pipex, i, NULL))
		{
			tmp = ft_strtrim(pipex->cmd[i][0], "/");
			if (!tmp)
				free_all_and_exit(pipex, "malloc");
			free(pipex->cmd[i][0]);
			pipex->cmd[i][0] = tmp;
		}
		i++;
	}
}

bool path_is_absolute(t_pipe *pipex, int nbr)
{
	return (pipex->cmd[nbr][0][0] == '.' || pipex->cmd[nbr][0][1] == '/');
}

void free_all_and_exit(t_pipe *pipex, const char *err)
{
	perror(err);
	free_all(pipex);
	exit(errno);
}
