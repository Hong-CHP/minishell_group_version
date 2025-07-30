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

void	print_fill_result(t_cmdlist **head_cmd, t_token **tok)
{
	t_cmdlist *cur;
	cur = *head_cmd;
	int pipe = 0;
	while(cur)
	{
		pipe++;
		if (cur->command->args && cur->command->args[cur->command->argc])
			printf("cur command args[argc] is %s\n", cur->command->args[cur->command->argc]);
		else
			printf("cur node has null data\n");
		cur = cur->next;
	}
	printf("pipe number is %d\n", pipe);
	t_token *cur_t;
	int count = 0;
	cur_t = *tok;
	while(cur_t)
	{
		count++;
		printf("cur_t type: %d, value : %s\n", cur_t->type, cur_t->value);
		cur_t = cur_t->next;
	}
	printf("token size is %d\n", count);

}

void	minishell(char *input, t_varlist **head_var, char **envp)
{
	t_parser	*parser;
	t_cmdlist	*head_cmd;
	t_pipex		*pipe_data;

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
	// print_fill_result(&head_cmd, &parser->tokens);
	if(if_pipex(&head_cmd) == 0 && if_buildin(head_cmd->command->cmd))
		execute_buildin(head_cmd->command, envp);
	if (if_pipex(&head_cmd) > 1)
	{
		init_pipe_data(pipe_data, envp);
		execute_pipeline(&head_cmd, parser, pipe_data);
	}
	// free_cmd(cmd_list);
	// free_parser(parser);
}