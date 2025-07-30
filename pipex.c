#include "minishell.h"
#include "libft.h"

void	init_pipe_data(t_pipex *pipe_data, char **envp)
{
	pipe_data->f_fds[0] = 0;
	pipe_data->f_fds[1] = 1;
	pipe_data->envp = envp;
	pipe_data->prev_pipe = -1;
	return (pipe_data);
}

void	create_pipe_fork_children()
{
	
}

void	execute_pipeline(t_cmdlist **head_cmd, t_parser *parser, t_pipex *pipe_data)
{
	t_cmdlist	*cur;

	if(get_in_out_files_fd(head_cmd, parser, pipe_data) != 0)
		return ;
	cur = *head_cmd;
	while (cur)
	{
		if(if_buildin(cur->command->cmd))
			execute_buildin(cur->command, pipe_data);
		creat_pipe_fork_children(cur->command, pipe_data);
		cur = cur->next;
	}
	wait_child_process();
}
