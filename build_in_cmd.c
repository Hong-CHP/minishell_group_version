#include "minishell.h"
#include "libft.h"

int		builtin_cd(t_command *cmd)
{
	char	*path;
	char	*home;

	if (cmd->argc == 1)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		path = home;
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

int		builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: pwd: error getting current directory\n", 2);
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}

//Iterate through env array and print each BUT
//Might need to handle arguments
int	builtin_env(char **ev)
{
	int	i;

	i = 0;
	while (ev[i])
	{
		ft_putstr_fd(ev[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

//I think this one is fine
// int		builtin_exit(t_command *cmd)
// {
// 	int	exit_code;

// 	exit_code = 0;
// 	if (cmd->argc > 1)
// 	{
// 		exit_code = ft_atoi(cmd->args[1]);
// 	}
// 	ft_putstr_fd("exit builtin\n", 1);
// 	exit(exit_code);
// }

int		builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->argc > 1 && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (i < cmd->argc)
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (i < cmd->argc - 1)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
	return (0);
}