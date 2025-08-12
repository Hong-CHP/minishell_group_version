#include "minishell.h"
#include "libft.h"

void	child_process(t_varlist **head_var, t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data)
{
	if (cur == *head_cmd && pipe_data->f_fds[0] != -1)
	{
		dup2(pipe_data->f_fds[0], 0);
		close(pipe_data->f_fds[0]);
	}
	else
	{
		dup2(pipe_data->prev_pipe, 0);
		close(pipe_data->prev_pipe);
	}
	if (cur->next == NULL && pipe_data->f_fds[1] != -1)
	{
		dup2(pipe_data->f_fds[1], 1);
		close(pipe_data->f_fds[1]);
	}
	else
	{
		dup2(pipe_data->pipefd[1], 1);
		close(pipe_data->pipefd[1]);
	}
	close(pipe_data->pipefd[0]);
	execute_cmd(head_var, cur->command, pipe_data->envp);
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

void	fork_and_pid(t_varlist **head_var, t_cmdlist **head_cmd, t_cmdlist *cur, t_pipex *pipe_data)
{
	pid_t		pid;

	if (cur->next != NULL)
	{
		if (pipe(pipe_data->pipefd) == -1)
		{
			perror("pipe");
			return ;
		}
	}
	printf("DEBUG: Pipe created [%d, %d]\n", pipe_data->pipefd[0], pipe_data->pipefd[1]);
	pid = fork();
	if (pid < 0)
	{
		perror("pipe");
		return ;
	}
 	printf("DEBUG: Forked child PID=%d\n", pid);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		child_process(head_var, head_cmd, cur, pipe_data);
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

void	execute_pipeline(t_varlist **head_var, t_cmdlist **head_cmd, t_pipex *pipe_data)
{
	t_cmdlist	*cur;

	cur = *head_cmd;
	while (cur)
	{
		fork_and_pid(head_var, head_cmd, cur, pipe_data);
		cur = cur->next;
	}
	g_exit_status = wait_child_process();
}
