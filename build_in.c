#include "minishell.h"
#include "libft.h"

int		if_buildin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

int		execute_builtin(t_command *cmd_node, char **ev)
{
	if (!cmd_node)
		return (0);
	if (ft_strcmp(cmd_node, "cd") == 0)
		return (builtin_cd(cmd_node));
	if (ft_strcmp(cmd_node, "echo") == 0)
		return (builtin_echo(cmd_node));
	if (ft_strcmp(cmd_node, "env") == 0)
		return (builtin_env(cmd_node, ev));
	if (ft_strcmp(cmd_node, "exit") == 0)
		return (builtin_exit(cmd_node));
	if (ft_strcmp(cmd_node, "pwd") == 0)
		return (builtin_pwd());
	return (1);
}