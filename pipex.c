#include "minishell.h"
#include "libft.h"

void	child_process(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data)
{
	if (cur == *head_cmd)
	{
		close(pipe_data->pipefd[0]);
		dup2(pipe_data->f_fds[0], 0);
		close(pipe_data->f_fds[0]);
	}
	else
	{
		dup2(pipe_data->prev_pipe, 0);
		close(pipe_data->prev_pipe);
	}
	if (cur->next == NULL)
	{
		dup2(pipe_data->f_fds[1], 1);
		close(pipe_data->f_fds[1]);
	}
	else
	{
		dup2(pipe_data->pipefd[1], 1);
		close(pipe_data->pipefd[1]);
	}
	if(if_buildin(cur->command->cmd))
		execute_builtin(cur->command, pipe_data->envp);
	else
		execute_cmd();
}

void	parent_process(t_cmdlist *cur, t_pipex *pipe_data)
{
	if (pipe_data->prev_pipe != -1)
		close(pipe_data->prev_pipe);
	if (cur->next != NULL)
	{
		close(pipe_data->pipefd[1]);
		pipe_data->prev_pipe = pipe_data->pipefd[0];
	}
}

void	fork_and_pid(t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data)
{
	pid_t		pid;

	if (pipe(pipe_data->pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("pipe");
		return ;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		child_process(head_cmd, cur, pipe_data);
	}
	else
		parent_process(cur, pipe_data);
}

int	wait_child_process(void)
{
	int	last_status;
	int	new_status;

	last_status = 0;
	while (wait(&new_status) > 0)
	{
		if (WIFEXITED(new_status))
			last_status = WEXITSTATUS(new_status);
		else if (WIFSIGNALED(new_status))
		{
			last_status = 128 + WTERMSIG(new_status);
			if (last_status == 130)
				ft_putchar_fd('\n', 1);
		}
	}
	return (last_status);
}

void	execute_pipeline(t_cmdlist **head_cmd, t_parser *parser, t_pipex *pipe_data)
{
	t_cmdlist	*cur;

	if(get_in_out_files_fd(head_cmd, parser, pipe_data) != 0)
		return ;
	cur = *head_cmd;
	while (cur)
	{
		fork_and_pid(head_cmd, cur, pipe_data);
		cur = cur->next;
	}
	wait_child_process();
}
