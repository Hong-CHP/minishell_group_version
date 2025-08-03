#include "minishell.h"
#include "libft.h"

char	**extract_path(char **ev)
{
	int		i;

	i = 0;
	while (ev[i])
	{
		return (ft_strnstr(ev[i], "PATH=", 5));
		i++;
	}
}

char	*find_exe_path(char **args, char **ev)
{
	char	*path;
	char	*exe_path;
	char	**ev_path;
	int		i;

	exe_path = NULL;
	ev_path = extract_path(ev);
	if (!ev_path)
		return (NULL);
	i = 0;
	while (ev_path[i])
	{
		path = get_path_after_join(args, ev_path, i);
		if (path)
		{
			if (access(path, X_OK) == 0)
			{
				exe_path = ft_strdup(path);
				free(path);
				break ;
			}
			free(path);
		}
		i++;
	}
	free_split(ev_path);
	return (exe_path);
}

void	execute_cmd(t_command *cmd, char **ev)
{
	(void)ev;
	char	**exe_path;

	int i = 0;
	while (cmd->args[i])
	{
		if (if_slash(cmd->args[i]) > 0 && access(cmd->args[i], X_OK) == 0)
			exe_path = ft_strdup(cmd->args[i]);
		else
			exe_path = find_exe_path(cmd, ev);
		i++;
	}
	
}