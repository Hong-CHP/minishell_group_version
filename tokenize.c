#include "minishell.h"
#include "libft.h"

void add_token(t_token **head, t_token *new_token)
{
	t_token *current;
	
	if (!*head)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token *create_token(t_tk_type type, char *value)
{
	t_token *tok;
	
	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	if (value)
		tok->value = ft_strdup(value);
	else
		tok->value = NULL;
	tok->next = NULL;
	return (tok);
}

t_token	*create_word_token(t_parser *parser, t_varlist **head_var)
{
	char	*word;
	t_token	*tok;

	word = extract_word(parser, head_var);
	if (!word)
		return (NULL);
	tok = create_token(TOKEN_WORD, word);
	free(word);
	if (!tok)
		return (NULL);
	return (tok);
}

t_token	*next_token(t_parser *parser, t_varlist **head_var)
{
    char c = parser->input[parser->pos];

	if (c == '|')
		return (create_pipe_token(parser));
	if (c == '<')
		return (create_input_token(parser));
	if (c == '>')
		return (create_output_token(parser));
	if (c == '&')
		return (create_and_token(parser));
	if (c == ';')
		return (create_simple_token(parser, TOKEN_SEMICOLON, ";"));
	if (c == '(')
		return (create_simple_token(parser, TOKEN_LPAREN, "("));
	if (c == ')')
		return (create_simple_token(parser, TOKEN_RPAREN, ")"));
	return (create_word_token(parser, head_var));
}

t_token	*tokenize(t_parser *parser, t_varlist **head_var)
{
	t_token *head;
	t_token *tok;

    head = NULL;
	while (parser->input[parser->pos] && !parser->error)
	{
		skip_whitespace(parser);
		if (!parser->input[parser->pos] || parser->error)
			break;
		tok = next_token(parser, head_var);
		if (!tok)
		{
			free_token_list(&head);
			return (NULL);
		}
		if (parser->error && !tok)
			return (head);
		if (tok)
			add_token(&head, tok);
	}
	add_token(&head, create_token(TOKEN_EOF, NULL));
	return (head);
}