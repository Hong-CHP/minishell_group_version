#include "minishell.h"
#include "libft.h"

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

char	*extract_if_single_quote(char *buf, t_parser *parser, int *len, int *dollar)
{
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != '\'')
	{
		if (parser->input[parser->pos] == '$')
			(*dollar)++;
		buf[(*len)++] = parser->input[parser->pos++];
	}
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
			parser->pos ++;
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


char	*extract_word(t_parser *parser, t_varlist **head_var)
{
	char	*buf;
	int		len;
	char	c;
	int		start_double;
	int		end_double;
	int		start_single;
	int		end_single;
	int		dollar;
	
	buf = malloc(sizeof(char) * (ft_strlen(parser->input) + 1));
	if (!buf)
		return (NULL);
	len = 0;
	dollar = 0;
	while (parser->input[parser->pos] && 
		   parser->input[parser->pos] != ' ' && 
		   parser->input[parser->pos] != '\t' && 
		   parser->input[parser->pos] != '\n' && 
		   parser->input[parser->pos] != '|' && 
		   parser->input[parser->pos] != '<' && 
		   parser->input[parser->pos] != '>' && 
		   parser->input[parser->pos] != '&' && 
		   parser->input[parser->pos] != ';' && 
		   parser->input[parser->pos] != '(' && 
		   parser->input[parser->pos] != ')')
	{
		c = parser->input[parser->pos];
		if (c == '\\')
		{
			printf("pos of slash start: %ld\n", parser->pos);
			if(!extract_if_slash(buf, parser, &len))
			{
				free(buf);
				return (NULL);
			}
			printf("pos of slash end: %ld\n", parser->pos);
		}
		else if (c == '\'')
		{
			start_single = parser->pos;
			printf("pos of single quote start: %d\n", start_single);
			if(!extract_if_single_quote(buf, parser, &len, &dollar))
			{
				free(buf);
				return (NULL);
			}
			end_single = parser->pos;
			if (dollar)
				printf("dollar %d sign existant in paire of single quote\n", dollar);
			printf("pos of single quote end: %d\n", end_single);
		}
		else if (c == '"')
		{
			start_double = parser->pos;
			printf("pos of double quote start: %d\n", start_double);
			if(!extract_if_double_quote(buf, parser, &len))
			{
				free(buf);
				return (NULL);
			}
			end_double = parser->pos;
			printf("pos of double quote end: %d\n", end_double);
		}
		else
			buf[len++] = parser->input[parser->pos++];
	}
	buf[len] = '\0';
	printf("parser->input is : %s\n", parser->input);
	printf("buf is : %s\n", buf);
	if (ft_strchr(parser->input, '$')
		&& (*(ft_strchr(parser->input, '$') - 1) != '\\')
		&& ft_strchr(buf, '$') && dollar == 0)
	{
		char *tmp = buf;
		buf = reg_dollar_sign(tmp, head_var);
		free(tmp);
	}
	return (buf);
}

// char	*get_word(t_parser *parser, int start, t_varlist **head_var)
// {
// 	int		len;
// 	char	*word;
// 	char	*tmp;
// 	// char	ch;

// 	len = parser->pos - start;
// 	printf("the parser->pos is: %ld, the start is %d\n", parser->pos, start);
// 	if (len == 0)
// 		return (NULL);
// 	word = malloc(sizeof(char) * (len + 1));
// 	if (!word)
// 		return (NULL);
// 	ft_strlcpy(word, &parser->input[start], len + 1);
// 	printf("actually the word is: %s\n", word);
	// ch = get_sign_in_the_word(word);
	// printf("actually the sign is: %c\n", ch);
	// if (ch == '\\' && if_slash_trans(word) == 1 
	// 	&& word[ft_strlen(word) - 1] != word[0])
	// {
	// 	printf("iicicicici\n");
	// 	tmp = word;
	// 	word = dup_str_without_slash(tmp, ft_strlen(word));
	// 	if (!word)
	// 	{
	// 		free(tmp);
	// 		return (NULL);
	// 	}
	// 	free(tmp);
	// }
	// else if (ch == '\'')
	// {
	// 	tmp = word;
	// 	word = find_words_in_single_quote(tmp, '\'');
	// 	if (!word)
	// 	{
	// 		free(tmp);
	// 		return (NULL);
	// 	}
	// 	free(tmp);
	// }
	// else if (ch == '\"')
	// else if (word[0] == '\\' && word[ft_strlen(word) - 2] == '\\')
	// {
	// 	tmp = word;
	// 	word = find_words_in_slash(tmp);
	// 	if (!word)
	// 	{
	// 		free(tmp);
	// 		return (NULL);
	// 	}
	// 	free(tmp);
	// }
	// else if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
	// {
	// 	tmp = word;
	// 	word = find_words_in_single_quote(tmp, '\'');
	// 	if (!word)
	// 	{
	// 		free(tmp);
	// 		return (NULL);
	// 	}
	// 	free(tmp);
	// }
	// else if (word[0] == '"' && word[ft_strlen(word) - 1] == '"')
	// {
	// 	tmp = word;
	// 	word = find_words_in_quote(tmp, '"');
	// 	if (!word)
	// 	{
	// 		free(tmp);
	// 		return (NULL);
	// 	}
	// 	free(tmp);
	// 	printf("find in quote, the word is: %s\n", word);
	// 	if (if_dollar_sign(word) > 0)
	// 	{
	// 		tmp = word;
	// 		word = reg_dollar_sign(word, head_var);
	// 		free(tmp);
	// 	}
	// 	printf("word after replaced is: %s\n", word);
	// }
	// if (if_dollar_sign(word) > 0)
	// {
	// 	tmp = word;
	// 	word = reg_dollar_sign(word, head_var);
	// 	free(tmp);
	// }
// 	return (word);
// }