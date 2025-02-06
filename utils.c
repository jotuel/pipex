/* ************************************************************************** */
/*                                                                            */
/*                                                     __     h  _ _  _| _    */
/*   utils.c                                        __/  \__  i (_(_)(_|(/_   */
/*                                                 /  \__/  \ v  _ _|_        */
/*   By: jtuomi <jtuomi@student.hive.fi>           \__/  \__/ e _>(_| | --    */
/*                                                 /  \__/  \ .  _  _ |       */
/*   Created: 2025/02/06 20:27:14 by jtuomi        \__/  \__/ f (_)(_)|       */
/*   Updated: 2025/02/06 20:31:33 by jtuomi           \__/    i               */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void util_parse_args(t_pipe *pipex)
{
    int i;

    i = 0;
    while(pipex->envp[i])
        ft_strnstr(pipex->envp[i++], "PATH=", 5);
    pipex->path = ft_split(&pipex->envp[i - 1][6], ':');
}
