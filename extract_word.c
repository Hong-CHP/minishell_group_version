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

	len = parser->pos - start;
	if (len == 0)
		return (NULL);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, &parser->input[start], len + 1);
	printf("actually the word is: %s\n", word);
	if (word[0] == '\'' && word[ft_strlen(word) - 1] == '\'')
		word = find_words_in_quote(word, '\'');
	else if (word[0] == '"' && word[ft_strlen(word) - 1] == '"')
	{
		word = find_words_in_quote(word, '"');
		printf("find in quote, the word is: %s\n", word);
		if (if_dollar_sign(word) > 0)
			word = reg_dollar_sign(word, head_var);
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
			extract_if_quote(c, parser);
		else
			parser->pos++;
	}
	return (get_word(parser, start, head_var));
}
