#include "minishell.h"
#include "libft.h"

void	clean_cmdlist(t_cmdlist **head)
{
	t_cmdlist	*cur;
	t_cmdlist	*next;

	cur = *head;
	while (cur)
	{
		next = cur->next;
		free(cur->command);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

void	add_cmdlist_back(t_cmdlist **head, t_cmdlist *cmd_node)
{
	t_cmdlist	*cur;

	cur = NULL;
	if (!*head)
		*head = cmd_node;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = cmd_node;
	}
}

t_command	*tokenize_cmd(t_parser *parser, t_varlist **head_var, t_cmdlist *node)
{
	t_varlist *cur = *head_var;
	while(cur)
	{
		if (cur->var_data && cur->var_data->var && cur->var_data->val)
			printf("cur content: %s = %s export %d\n", cur->var_data->var, cur->var_data->val, cur->var_data->exported);
		else
			printf("cur node has null data\n");
		cur = cur->next;
	}
	while (parser->current && parser->current->type == TOKEN_WORD)
	{
		if (ft_strchr(parser->current->value, '=')
			|| if_export_variable(parser->current->value))
			create_var_list_or_find_node(head_var, parser->input);
		else
			printf("parser type-value is %d: %s\n", parser->current->type, parser->current->value);
		// fill_node_cmd_args(node, parser);
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
	node->command->args = NULL;
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
	tokenize_cmd(parser, head_var, node);
	if (!node->command)
	{
		free(node);
		return (0);
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
	parser->current = parser->tokens;
	if (parser->error)
		return (NULL);
	return (parse_pipeline(parser, head_var));
}