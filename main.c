#include "minishell.h"
#include "libft.h"
#include <signal.h>

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_sigaction(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	read_the_line(char *input, t_varlist **head_var, char **envp)
{
	while (1)
	{
		input = readline("\1\033[1;33m\2minishell\1\033[0m\2> ");
		if (input == NULL)
		{
			rl_on_new_line();
			printf("exit\n");
			break ;
		}
		else if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		else if (ft_strcmp(input, "history -c") == 0)
			rl_clear_history(); 
		else if (*input)
		{
			add_history(input);
			minishell(input, head_var, envp);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_varlist	*head_var;
	(void)		argv;
	
	if (argc != 1)
		return (1);
	head_var = NULL;
	input = NULL;
	setup_sigaction();
	read_the_line(input, &head_var, envp);
	clean_var_list(&head_var);
	return (0);
}
