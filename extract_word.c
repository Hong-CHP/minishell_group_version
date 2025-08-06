#include "minishell.h"
#include "libft.h"

void	extract_if_quote(char c, t_parser *parser)
{
	char	quote;

	quote = '\0';
	quote = c;
	parser->pos++;
	while (parser->input[parser->pos]
		&& parser->input[parser->pos] != quote)
		parser->pos++;
	if (parser->input[parser->pos] == quote)
		parser->pos++;
}

char	*get_word(t_parser *parser, int start, t_varlist **head_var)
{
	int		len;
	char	*word;
	char	*tmp;

	len = parser->pos - start;
	printf("the parser->pos is: %ld, the start is %d\n", parser->pos, start);
	if (len == 0)
		return (NULL);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, &parser->input[start], len + 1);
	printf("actually the word is: %s\n", word);
	if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
	{
		tmp = word;
		word = find_words_in_quote(tmp, '\'');
		free(tmp);
	}
	else if (word[0] == '"' && word[ft_strlen(word) - 1] == '"')
	{
		tmp = word;
		word = find_words_in_quote(tmp, '"');
		free(tmp);
		printf("find in quote, the word is: %s\n", word);
		if (if_dollar_sign(word) > 0)
		{
			tmp = word;
			word = reg_dollar_sign(word, head_var);
			free(tmp);
		}
		printf("word after replaced is: %s\n", word);
	}
	return (word);
}

char	*extract_word(t_parser *parser, t_varlist **head_var)
{
	int		start;
	char	c;
	(void)head_var;

	start = parser->pos;
	c = parser->input[parser->pos];
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
		if (c == '\'' || c == '"')
		{
			extract_if_quote(c, parser);
		}
		else
			parser->pos++;
	}
	return (get_word(parser, start, head_var));
}
