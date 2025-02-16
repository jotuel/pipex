/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   utils.c                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/06 20:27:14 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/16 19:36:23 by jtuomi           ###   ########.fr       */
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
	tmp = ft_strjoin("/", pipex->cmd[0][0]);
	free(pipex->cmd[0][0]);
	pipex->cmd[0][0] = tmp;
	tmp = ft_strjoin("/", pipex->cmd[1][0]);
	free(pipex->cmd[1][0]);
	pipex->cmd[1][0] = tmp;
	if (!commands_in_path(pipex, 0, 0, NULL))
	{
		tmp = ft_strtrim(pipex->cmd[0][0], "/");
		free(pipex->cmd[0][0]);
		pipex->cmd[0][0] = tmp;
		tmp = ft_strtrim(pipex->cmd[0][1], "/");
		free(pipex->cmd[0][1]);
		pipex->cmd[0][1] = tmp;
	}
}
