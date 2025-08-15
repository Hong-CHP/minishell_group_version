#include "minishell.h"
#include "libft.h"
#include <signal.h>

int g_exit_status = 0;

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

//review if next line with ctrl+c ctrl+d as input
int		read_content_unclose_quote(char **input)
{
	char	*next_line;
	char	*tmp;

	while (has_unclosed_quote(*input))
	{
		next_line = readline("\1\033[1;33m\2>\1\033[0m\2> ");
		if (next_line == NULL)
		{
            free(*input);
            *input = NULL;
            rl_on_new_line();
            printf("exit\n");
            return 0;
        }
		tmp = *input;
		*input = ft_strjoin(tmp, "\n");
		free(tmp);
		tmp = *input;
		*input = ft_strjoin(tmp, next_line);
		free(tmp);
		free(next_line);
	}
	return (1);
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
			// if(has_unclosed_quote(input))
			// 	break ;
			add_history(input);
			minishell(input, head_var, envp);
		}
		free(input);
	}
	if (input)
		free(input);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_varlist	*head_var;
	int		envp_size;
	int		i;
	(void)		argv;
	
	if (argc != 1)
		return (1);
	envp_size = count_size_of_envp(envp);
	head_var = NULL;
	input = NULL;
	setup_sigaction();
	i = 0;
	while (i < envp_size)
	{
		create_var_list(&head_var, envp[i]);
		set_varlist_exported(&head_var);
		i++;
	}
	read_the_line(input, &head_var, envp);
	clean_var_list(&head_var);
	return (0);
}
