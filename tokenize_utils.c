#include "minishell.h"
#include "libft.h"

int		if_quote(char *str)
{
	int		i;
	char	ch;

	i = 0;
	while (str[i] && str[i] != '\''&& str[i] != '"')
		i++;
	if (str[i] == '\'' || str[i] == '"')
	{
		ch = str[i];
		i++;
	}
	else
		return (0);
	while (str[i] && str[i] != ch)
		i++;
	if (str[i] == ch)
		return ((int)ch);
	if (!str[i])
		return (-1);
	return (0);
}

void	free_token_list(t_token **head)
{
	t_token	*cur;
	t_token	*next;

	cur = *head;
	while (cur && cur->next)
	{
		next = cur->next;
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = next;
	}
	*head = NULL;
}

char	*extract_value_if_sign(char *value, t_varlist **head_var)
{
	char	*buf;
	int		i;
	int		len;
	int		dollar;
	int		start;
	int		end;
	char 	*part;
	char	*pos;
	int		len_part;
	char	*front;
	char	*tmp;
	char	*joined;

	buf = malloc(sizeof(char) * (ft_strlen(value) + 1));
	if (!buf)
		return (NULL);
	len = 0;
	start = 0;
	end = 0;
	i = 0;
	dollar = 0;
	while (value[i])
	{
		if (value[i] == '\\')
		{
			i++;
			if (!value[i])
			{
				printf("incorrect syntaxe\n");
				return (NULL);
			}
			else
				buf[len++] = value[i++];
		}
		else if (value[i] == '\'')
		{
			i++;
			start = i;
			while (value[i] && value[i] != '\'')
			{
				if (value[i] == '$')
					dollar++;
				buf[len++] = value[i++];
			}
			if (value[i] == '\'')
			{
				i++;
				end = i - 1;
			}
			else
			{
				printf("incorrect syntaxe\n");
				return (NULL);
			}
		}
		else if (value[i] == '"')
		{
			i++;
			while (value[i] && value[i] != '"')
			{
				if (value[i] == '\\' && (value[i + 1] == '"'
					|| value[i + 1] == '$' || value[i + 1] == '\\'))
					i++;
				buf[len++] = value[i++];
			}
			if (value[i] == '"')
				i++;
			else if (!value[i])
			{
				printf("here print: incorrect syntaxe\n");
				return (NULL);
			}
		}
		else
			buf[len++] = value[i++];
	}
	buf[len] = '\0';
	if (start > 0 && end > 0 && dollar > 0)
	{
		part = ft_substr(buf, start, end - start);
		if (!part)
			return (NULL);
		len_part = ft_strlen(part);
		pos = ft_strnstr(buf, part, ft_strlen(buf));
		front = ft_substr(buf, 0, pos - buf);
		if (ft_strchr(front, '$'))
		{
			tmp = front;
			front = reg_dollar_sign(front, head_var);
			free(tmp);
		}
		joined = ft_strjoin(front, part);
		free(front);
		free(part);
		if (*(pos + len_part))
		{
			part = ft_substr(pos, len_part, ft_strlen(pos) - len_part);
			if (ft_strchr(part, '$'))
			{
				tmp = part;
				part = reg_dollar_sign(tmp, head_var);
				free(tmp);
			}
			tmp = joined;
			joined = ft_strjoin(tmp, part);
			free(tmp);
			free(part);
		}
		free(buf);
		buf = joined;
	}
	else if (ft_strchr(buf, '$') && dollar == 0)
	{
		tmp = buf;
		buf = reg_dollar_sign(buf, head_var);
		free(tmp);
	}
	return (buf);
}