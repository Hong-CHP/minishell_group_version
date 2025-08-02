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
	pipe_data = malloc(sizeof(t_pipex));
	if (!pipe_data)
		return (NULL);
	pipe_data->f_fds[0] = 0;
	pipe_data->f_fds[1] = 1;
	pipe_data->envp = envp;
	pipe_data->prev_pipe = -1;
	return (pipe_data);
}

int		get_in_out_files_fd(t_cmdlist **head, t_parser *p, t_pipex *pipe_data)
{
	t_cmdlist *cur;

	cur = *head;
	while (cur)
	{
		if (cur->command->infile)
		{
			printf("cur->infile is %s\n", cur->command->infile);
			pipe_data->f_fds[0] = open(cur->command->infile, O_RDONLY);
		}
		if (cur->command->outfile)
		{
			printf("cur->outfile is %s\n", cur->command->outfile);
			pipe_data->f_fds[1] = open(cur->command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (pipe_data->f_fds[0] < 0 || pipe_data->f_fds[1] < 0)
			return (set_error(p));
		cur = cur->next;
	}
	return (0);
}