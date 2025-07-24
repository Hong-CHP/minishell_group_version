#include "minishell.h"
#include "libft.h"

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
	// t_parser *parser;
	// t_cmdlist *head_cmd;

	if (!input || !*input)
		return;
	// parser = init_parser(input);
	// if (!parser)
	// 	return;
	if (ft_strchr(input, '=') || if_export_variable(input))
	{
		create_var_list_or_find_node(head_var, input);
		// free_parser(parser);
		// return ;
	}
	if (head_var && *head_var)
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
	}
	// cmd_list = parse_cmd_line(parser);
	// if (!cmd_list)
	// {
	// 	if (parser->error && parser->error_msg)
	// 	{
	// 		ft_putstr_fd("minishell: ", 2);
	// 		ft_putstr_fd(parser->error_msg, 2);
	// 		ft_putstr_fd("\n", 2);
	// 	}
	// 	free_parser(parser);
	// 	return;
	// }
	
	// exec.envp = envp;
	// exec.prev_pipe = -1;
	
	// execute_pipeline(cmd_list, &exec);
	
	// free_cmd(cmd_list);
	// free_parser(parser);
}