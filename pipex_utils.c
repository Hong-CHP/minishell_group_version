#include "minishell.h"
#include "libft.h"

int		if_pipex(t_cmdlist **head_cmd)
{
	t_cmdlist	*cur;
	int			pipe; 
	
	pipe = 0;
	cur = *head_cmd;
	while(cur)
	{
		pipe++;
		cur = cur->next;
	}
	return (pipe);
}

t_pipex	*init_pipe_data(t_pipex *pipe_data, char **envp)
{
	pipe_data->f_fds[0] = -1;
	pipe_data->f_fds[1] = -1;
	pipe_data->envp = envp;
	pipe_data->prev_pipe = -1;
	return (pipe_data);
}

int		get_out_files_fd(t_cmdlist *last, t_pipex *pipe_data)
{
	if (last->command->outfile)
	{
		pipe_data->f_fds[1] = open(last->command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipe_data->f_fds[1] < 0)
		{
			perror("open file failed");
			if (pipe_data->f_fds[0] > 0)
				close(pipe_data->f_fds[0]);
			return (0);
		}
	}
	return (1);
}

int		get_in_out_files_fd(t_cmdlist **head, t_pipex *pipe_data)
{
	t_cmdlist	*first;
	t_cmdlist	*last;

	first = *head;
	last = *head;
	while (last && last->next)
		last = last->next;
	if (first->command->infile)
	{
		pipe_data->f_fds[0] = open(first->command->infile, O_RDONLY);
		if (pipe_data->f_fds[0] < 0)
		{
			perror("open file failed");
			return (0);
		}
	}
	if (!get_out_files_fd(last, pipe_data))
		return (0);
	return (1);
}
