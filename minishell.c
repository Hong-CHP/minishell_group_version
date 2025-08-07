#include "minishell.h"
#include "libft.h"

void	free_cmdlist(t_cmdlist **head_cmd)
{
	t_cmdlist *cur;
	t_cmdlist *next;

	cur = *head_cmd;
	while (cur)
	{
		next = cur->next;
		if (cur->command && cur->command->args)
		{
			while (cur->command->argc >= 0)
			{
				free(cur->command->args[cur->command->argc]);
				cur->command->argc--;
			}
			free(cur->command->args);
		}
		if (cur->command->infile)
			free(cur->command->infile);
		if (cur->command->outfile)
			free(cur->command->outfile);
		free(cur->command);
		free(cur);
		cur = next;
	}
}

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

void	print_fill_result(t_varlist **head_var, t_cmdlist **head_cmd, t_token **tok)
{
	t_varlist *cur_var;
	cur_var = *head_var;
	while(cur_var)
	{
		if (cur_var->var_data)
			printf("cur_var data var: %s (val: %s)\n", cur_var->var_data->var, cur_var->var_data->val);
		else
			printf("cur_var node has null data\n");
		cur_var = cur_var->next;
	}
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

void	execute_cmd_or_cmds(t_parser *parser, t_cmdlist **head_cmd, t_pipex	*pipe_data)
{
	(void)parser;
	if (if_pipex(head_cmd) > 1)
	{
		if ((*head_cmd)->command->here_doc == 1)
		{
			if (!execute_here_doc(head_cmd, pipe_data))
				return ;
		}
		else
		{
			if(!get_in_out_files_fd(head_cmd, pipe_data))
				return ;
		}
		execute_pipeline(head_cmd, pipe_data);
		if ((*head_cmd)->command->here_doc == 1)
			unlink("here.txt");
	}
	else
	{
		printf("executing single commande: \n");
		if (if_buildin((*head_cmd)->command->cmd))
		{}
			// execute_builtin((*head_cmd)->command, envp);
		else
		{
			if ((*head_cmd)->command->here_doc == 1)
			{
				if (!execute_here_doc(head_cmd, pipe_data))
					return ;
			}
			else
			{
				if(!get_in_out_files_fd(head_cmd, pipe_data))
					return ;
			}
			execute_single_cmd(head_cmd, (*head_cmd)->command, pipe_data);
			if ((*head_cmd)->command->here_doc == 1)
				unlink("here.txt");
		}
	}
}

void	minishell(char *input, t_varlist **head_var, char **envp)
{
	t_parser	*parser; //malloc
	t_cmdlist	*head_cmd;//malloc
	t_pipex		*pipe_data;

	if (!input || !*input)
		return ;
	t_varlist *cur_var;
	cur_var = *head_var;
	while(cur_var)
	{
		if (cur_var->var_data)
			printf("cur_var data var: %s (val: %s)\n", cur_var->var_data->var, cur_var->var_data->val);
		else
			printf("cur_var node has null data\n");
		cur_var = cur_var->next;
	}
	if (ft_strchr(input, '=') || if_export_variable(input))
	{
		create_var_list_or_find_node(head_var, input, envp);
		return ;
	}
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
	print_fill_result(head_var, &head_cmd, &(parser->tokens));
	pipe_data = malloc(sizeof(t_pipex));
	if (!pipe_data)
	{
		free_parser(parser);
		return ;
	}
	init_pipe_data(pipe_data, envp);
	execute_cmd_or_cmds(parser, &head_cmd, pipe_data);
	free_cmdlist(&head_cmd);
	free_parser(parser);
	free(pipe_data);
}