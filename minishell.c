#include "minishell.h"
#include "libft.h"

void	free_parser(t_parser *parser)
{
	t_token	*current;
	t_token	*next;

	if (!parser)
		return ;
	if (parser->input)
		free(parser->input);
	if (parser->error_msg)
		free(parser->error_msg);
	current = parser->tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	free(parser);
}

t_parser	*init_parser(char *input)
{
	t_parser *parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->input = ft_strdup(input);
	if (!parser->input)
	{
		free(parser);
		return (NULL);
	}
	parser->tokens = NULL;
	parser->current = NULL;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
	return (parser);
}

void	minishell(char *input, t_varlist **head_var, char **envp)
{
	(void)envp;
	t_parser *parser;
	t_cmdlist *head_cmd;

	if (!input || !*input)
		return;
	parser = init_parser(input);
	if (!parser)
		return;
	head_cmd = parse_cmd_line(parser, head_var);
	if (!head_cmd)
	{
		if (parser->error && parser->error_msg)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(parser->error_msg, 2);
			ft_putstr_fd("\n", 2);
		}
		free_parser(parser);
		return;
	}
	// exec.envp = envp;
	// exec.prev_pipe = -1;
	
	// execute_pipeline(cmd_list, &exec);
	
	// free_cmd(cmd_list);
	// free_parser(parser);
}