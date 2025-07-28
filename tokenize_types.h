// tokenize_type.h

#ifndef TOKENIZE_TYPE_H
# define TOKENIZE_TYPE_H

# include "minishell.h"

static t_token *create_pipe_token(t_parser *parser);
static t_token *create_input_token(t_parser *parser);
static t_token *create_output_token(t_parser *parser);
static t_token *create_and_token(t_parser *parser);
static t_token *create_simple_token(t_parser *parser, int type, const char *s);

#endif
