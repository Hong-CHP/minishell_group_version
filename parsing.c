#include "minishell.h"
#include "libft.h"

t_command	*tokenize_cmd(t_parser *parser, t_varlist **head_var, t_cmdlist *node)
{
	(void)head_var;
	while (parser->current && is_cmd_token(parser->current->type))
	{
		if (parser->current->type == TOKEN_WORD)
		{
			// if (ft_strchr(parser->current->value, '=')
			// 	|| if_export_variable(parser->current->value))
			// 	create_var_list_or_find_node(head_var, parser->input);
			// else
			// {
				handle_word(parser, node->command->args, &(node->command->argc));
				node->command->cmd = node->command->args[0];
				node->command->args[node->command->argc] = NULL;
				printf("args[%d] is : %s\n", node->command->argc - 1, node->command->args[node->command->argc - 1]);
			// }
		}
		else
		{
			printf("the token type %d(value : %s)\n", parser->current->type, parser->current->value);
			if (process_token(parser, node))
				return (NULL);
		}
		parser->current = parser->current->next;
	}
	return (node->command);
}

t_command	*create_init_cmd(t_cmdlist *node)
{
	node->command = (t_command *)malloc(sizeof(t_command));
	if (!node->command)
	{
		free(node);
		return (NULL);
	}
	ft_memset(node->command, 0, sizeof(t_command));
	node->command->cmd = NULL;
	node->command->args = malloc(sizeof(char *) * 256);
	if (!node->command->args)
	{
		free(node->command);
		return (NULL);
	}
	node->command->argc = 0;
	node->command->infile = NULL;
	node->command->outfile = NULL;
	node->command->append = 0;
	node->command->here_doc = 0;
	node->command->limiter = NULL;
	return (node->command);
}

t_cmdlist	*parse_simple_cmd(t_parser *parser, t_varlist **head_var)
{
	t_cmdlist	*node;

	node = (t_cmdlist *)malloc(sizeof(t_cmdlist));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_cmdlist));
	create_init_cmd(node);
	if (head_var && *head_var)
		node->var_list = *head_var;
	else
		node->var_list = NULL;
	if (!tokenize_cmd(parser, head_var, node))
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_cmdlist	*parse_pipeline(t_parser *parser, t_varlist **head_var)
{
	t_cmdlist	*head;
	t_cmdlist	*cmd_node;

	head = NULL;
	while (parser->current && parser->current->type != TOKEN_EOF)
	{
		cmd_node = parse_simple_cmd(parser, head_var);
		if (!cmd_node || parser->error)
		{
			clean_cmdlist(&head);
			return (NULL);
		}
		add_cmdlist_back(&head, cmd_node);
		if (parser->current && parser->current->type == TOKEN_PIPE)
			parser->current = parser->current->next;
		else
			break;
	}
	return (head);
}

t_cmdlist	*parse_cmd_line(t_parser *parser, t_varlist **head_var)
{
	parser->tokens = tokenize(parser, head_var);
	printf("parser->tokens->ytpe and value is %d, %s\n", parser->tokens->type, parser->tokens->value);
	parser->current = parser->tokens;
	if (parser->error)
		return (NULL);
	return (parse_pipeline(parser, head_var));
}