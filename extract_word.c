#include "minishell.h"
#include "libft.h"
#include <string.h>

char	*extract_if_slash(char *buf, t_parser *parser, int *len)
{
	parser->pos++;
	if (!parser->input[parser->pos])
	{
		printf("incorrect syntaxe\n");
		return (NULL);
	}
	if (parser->input[parser->pos])
		buf[(*len)++] = parser->input[parser->pos++];
	return (buf);
}

char	*extract_if_single_quote(char *buf, t_parser *parser, int *len, int *s_quot)
{
	parser->pos++;
	*s_quot = parser->pos;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '\'')
		buf[(*len)++] = parser->input[parser->pos++];
	if (parser->input[parser->pos] == '\'')
	{
		parser->pos++;
		return (buf);
	}
	else
	{
		printf("unclosed quote\n");
		return (NULL);
	}
}

char	*extract_if_double_quote(char *buf, t_parser *parser, int *len)
{
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '"')
	{
		if (parser->input[parser->pos] == '\\'
			&& (parser->input[parser->pos + 1] == '"'
				|| parser->input[parser->pos + 1] == '\\'
				|| parser->input[parser->pos + 1] == '$'))
			parser->pos++;
		buf[(*len)++] = parser->input[parser->pos++];
	}
	if (parser->input[parser->pos] == '"')
		parser->pos++;
	else if (!parser->input[parser->pos])
	{
		printf("incorrect syntaxe\n");
		return (NULL);
	}
	return (buf);
}


void	update_dollar_inside_single_quotes(t_parser *parser, int s_quot, int e_quot, int *dollar)
{
	int	i;

	i = s_quot;
	while (i < e_quot)
	{
		if (parser->input[i] == '$')
			(*dollar)++;
		i++;
	}
}

char	*join_back_content_after_quote(t_varlist **head_var, char *pos_after, int len, char *joined)
{
	char	*tmp;
	char	*back;

	tmp = NULL;
	back = ft_substr(pos_after, len, ft_strlen(pos_after) - len);
	if (ft_strchr(back, '$'))
	{
		tmp = back;
		back = reg_dollar_sign(tmp, head_var);
		free(tmp);
	}
	tmp = joined;
	joined = ft_strjoin(tmp, back);
	free(tmp);
	free(back);
	return (joined);
}

char	*when_dollar_inside_single_quote(char *buf, char *part, t_varlist **head)
{
	char	*pos_after_qt;
	int		len_in_qt;
	char	*front;
	char	*tmp;
	char	*joined;

	tmp = NULL;
	pos_after_qt = ft_strnstr(buf, part, ft_strlen(buf));
	printf("actually pos str is : %s\n", pos_after_qt);
	len_in_qt = ft_strlen(part);
	front = ft_substr(buf, 0, pos_after_qt - buf);
	if (ft_strchr(front, '$'))
	{
		tmp = front;
		front = reg_dollar_sign(tmp, head);
		free(tmp);
	}
	joined = ft_strjoin(front, part);
	free(front);
	free(part);
	printf("new content is %s\n", joined);
	if (*(pos_after_qt + len_in_qt))
		joined = join_back_content_after_quote(head, pos_after_qt, len_in_qt, joined);
	printf("joined is actually: %s\n", joined);
	return (joined);
}

int is_separator(char c)
{
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '|'  || c == '<'  || c == '>'  ||
            c == '&'  || c == ';'  ||
            c == '('  || c == ')');
}

char	*extract_by_type_sign(char *buf, t_parser *parser, int *s_quot, int *e_quot)
{
	int		len;

	len = 0;
	while (parser->input[parser->pos] && !is_separator(parser->input[parser->pos]))
	{
		if (parser->input[parser->pos] == '\\')
		{
			if(!extract_if_slash(buf, parser, &len))
				return (NULL);
		}
		else if (parser->input[parser->pos] == '\'')
		{
			if(!extract_if_single_quote(buf, parser, &len, s_quot))
				return (NULL);
			*e_quot = parser->pos - 1;
		}
		else if (parser->input[parser->pos] == '"')
		{
			if (!extract_if_double_quote(buf, parser, &len))
				return (NULL);
		}
		else
			buf[len++] = parser->input[parser->pos++];
	}
	buf[len] = '\0';
	return (buf);
}

char *handle_single_quote_dollar(char *buf, char *input, int doll_quot[3], t_varlist **head_var)
{
	char	*part;
	char	*tmp;
	
	part = ft_substr(input, doll_quot[0], doll_quot[1] - doll_quot[0]);
	if (!part)
		return (NULL);
	tmp = when_dollar_inside_single_quote(buf, part, head_var);
	free(buf);
	if (!tmp)
		return (NULL);
	return tmp;
}

char *extract_word(t_parser *parser, t_varlist **head_var)
{
	char	*buf;
	char	*tmp;
	int		doll_quot[3];

	buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!buf)
		return (NULL);
	ft_memset(doll_quot, 0, sizeof(doll_quot));
	if (!extract_by_type_sign(buf, parser, &doll_quot[0], &doll_quot[1]))
		return (NULL);
	update_dollar_inside_single_quotes(parser, doll_quot[0], doll_quot[1], &doll_quot[2]);
	if (doll_quot[0] && doll_quot[1] && doll_quot[2] > 0)
		buf = handle_single_quote_dollar(buf, parser->input, doll_quot, head_var);
	else if (ft_strchr(buf, '$') && doll_quot[2] == 0)
	{
		tmp = buf;
		buf = reg_dollar_sign(tmp, head_var);
		free(tmp);
	}
	return (buf);
}
