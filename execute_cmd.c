#include "minishell.h"
#include "libft.h"

char	**extract_path(char **ev)
{
	int		i;
	char	**ev_path;

	i = 0;
	while (ev[i])
	{
		if (ft_strncmp("PATH=", ev[i], 5) == 0)
		{
			ev_path = find_sign_then_split(ev[i] + 5);
			if (!ev_path)
			{
				perror("Extract path no success");
				free_split(ev_path);
				return (NULL);
			}
			return (ev_path);
		}
		i++;
	}
	return (NULL);
}

char	*get_path_after_join(char **args, char	**ev_path, int i)
{
	char	*temp;
	char	*path;

	temp = NULL;
	path = NULL;
	temp = ft_strjoin("/", args[0]);
	if (!temp)
	{
		free_split(ev_path);
		return (NULL);
	}
	path = ft_strjoin(ev_path[i], temp);
	free(temp);
	return (path);
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
	char	*exe_path;

	if (if_slash(cmd->args[0]) > 0 && access(cmd->args[0], X_OK) == 0)
		exe_path = ft_strdup(cmd->args[0]);
	else
		exe_path = find_exe_path(cmd->args, ev);
	fprintf(stderr, "exe_path is %s\n", exe_path);
	if (!exe_path)
	{
		perror("No vailable command or path");
		exit (127);
	}
	if (exe_path)
	{
		execve(exe_path, cmd->args, ev);
		perror("Error executions");
		free(exe_path);
		exit(126);
	}
	free(exe_path);
}

void	execute_single_cmd( t_cmdlist **head_cmd, t_command *cmd, t_pipex *pipe_data)
{
	(void)head_cmd;
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return ;
	}
	if (pid == 0)
	{
		dup2(pipe_data->f_fds[0], 0);
		close(pipe_data->f_fds[0]);
		dup2(pipe_data->f_fds[1], 1);
		close(pipe_data->f_fds[1]);
		execute_cmd(cmd, pipe_data->envp);
	}
	else
		waitpid(pid, NULL, 0);
}
