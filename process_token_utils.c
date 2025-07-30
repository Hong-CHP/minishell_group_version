#include "minishell.h"
#include "libft.h"

int	set_error(t_parser *p)
{
	if (p->error_msg)
		free(p->error_msg);
	p->error = 1;
	p->error_msg = ft_strdup("Syntax error");
	return (1);
}

int	is_cmd_token(int type)
{
	return (type == TOKEN_WORD
		 || type == TOKEN_REDIR_IN
		 || type == TOKEN_REDIR_OUT
		 || type == TOKEN_REDIR_APPEND
		 || type == TOKEN_REDIR_HEREDOC);
}