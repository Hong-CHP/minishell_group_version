#include "minishell.h"
#include "libft.h"

int		if_buildin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	// if (ft_strcmp(cmd, "echo") == 0)
	// 	return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	return (0);
}

int		builtin_unset(t_varlist **head_var, t_command *cmd_node)
{
	t_varlist	*cur;
	t_varlist	*prev;

	cur = *head_var;
	prev = NULL;
	while (cur && cur->next)
	{
		if (ft_strcmp(cur->var_data->var, cmd_node->args[1]) == 0
			&& cur->var_data->exported == 1)
		{
			if (prev)
				prev->next = cur->next;
			else
				*head_var = cur->next;
			free(cur->var_data->val);
			free(cur->var_data->var);
			free(cur->var_data);
			free(cur);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

int		execute_builtin(t_varlist **head_var, t_command *cmd_node, char **ev)
{
	(void)ev;
	if (!cmd_node || !cmd_node->args)
		return (0);
	if (ft_strcmp(cmd_node->args[0], "cd") == 0)
		return (builtin_cd(cmd_node));
	// if (ft_strcmp(cmd_node->args[0], "echo") == 0)
	// 	return (builtin_echo(cmd_node));
	if (ft_strcmp(cmd_node->args[0], "unset") == 0)
	{
		if (!cmd_node->args[1] || cmd_node->args[1][0] == '\0')
			return (0);
		return (builtin_unset(head_var, cmd_node));
	}
	if (ft_strcmp(cmd_node->args[0], "env") == 0)
		return (builtin_env(ev));
	if (ft_strcmp(cmd_node->args[0], "exit") == 0)
		return (builtin_exit(cmd_node));
	if (ft_strcmp(cmd_node->args[0], "pwd") == 0)
		return (builtin_pwd());
	return (1);
}