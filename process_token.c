#include "minishell.h"
#include "libft.h"

int	handle_word(t_parser *p, char **args, int *argc)
{
	args[*argc] = ft_strdup(p->current->value);
	if (!args[*argc])
		return (1);
	(*argc)++;
	return (0);
}

int		handle_infile(t_parser *p, t_cmdlist *node)
{
	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p));
	if (node->command->infile)
		free(node->command->infile);
	node->command->infile = ft_strdup(p->current->value);
	if (!node->command->infile)
		return (1);
	if (check_infile_permission(p, node->command->infile) != 0)
	{
		free(node->command->infile);
		return (1);	
	}
	printf("node->command->infile name is: %s\n", node->command->infile);
	return (0);
}

int		handle_outfile(t_parser *p, t_cmdlist *node, int append)
{
	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p));
	if (node->command->outfile)
		free(node->command->outfile);
	node->command->outfile = ft_strdup(p->current->value);
	if (!node->command->outfile)
		return (1);
	if (check_outfile_permission(p, node->command->outfile) != 0)
	{
		free(node->command->outfile);
		return (1);	
	}
	printf("node->command->outfile name is: %s\n", node->command->outfile);
	node->command->append = append;
	return (0);
}

int	handle_heredoc_parse(t_parser *p, t_cmdlist *node)
{
	p->current = p->current->next;
	if (!p->current || p->current->type != TOKEN_WORD)
		return (set_error(p));
	if (node->command->here_doc != 0)
		free(node->command->limiter);
	node->command->limiter = ft_strdup(p->current->value);
	if (!node->command->limiter)
		return (1);
	printf("node->command->limiter word is: %s\n", node->command->limiter);
	node->command->here_doc = 1;
	return (0);
}

int		process_token(t_parser *p, t_cmdlist *node)
{
	int err;

	err = 0;
	if (p->current->type == TOKEN_REDIR_IN)
		err = handle_infile(p, node);
	else if (p->current->type == TOKEN_REDIR_OUT)
		err = handle_outfile(p, node, 0);
	else if (p->current->type == TOKEN_REDIR_APPEND)
		err = handle_outfile(p, node, 1);
	else if (p->current->type == TOKEN_REDIR_HEREDOC)
		err = handle_heredoc_parse(p, node);
	return (err);
}