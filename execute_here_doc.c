#include "minishell.h"
#include "libft.h"

int	creat_write_in_tmp_file(t_command *cmd)
{
	char	*limiter;
	int		tmpfile;
	char	*line;

	limiter = cmd->limiter;
	tmpfile = open("here.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmpfile < 0)
		return (tmpfile);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(tmpfile, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	return (tmpfile);
}

int		execute_here_doc(t_cmdlist **head_cmd, t_pipex *pipe_data)
{
	t_cmdlist	*last;
	int		tmpfile;

	last = *head_cmd;
	while (last && last->next)
		last = last->next;
	tmpfile = 1;
	if (last->command->outfile)
	{
		pipe_data->f_fds[1] = open(last->command->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipe_data->f_fds[1] < 0)
			return (0);
	}
	tmpfile = creat_write_in_tmp_file((*head_cmd)->command);
	if (tmpfile < 0)
		return (0);
	close(tmpfile);
	tmpfile = open("here.txt", O_RDONLY);
	if (tmpfile < 0)
		return (0);
	pipe_data->f_fds[0] = tmpfile;
	return(1);
}
