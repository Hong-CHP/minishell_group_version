#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_tk_type 
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF,
	TOKEN_ERROR,
}				t_tk_type;

typedef struct s_token 
{
    t_tk_type		type;
    char			*value;
    struct s_token	*next;
}				t_token;

typedef struct s_parser
{
	t_token *tokens;
    t_token *current;
	char	*input;
	size_t	pos;
	bool	error;
	char	*error_msg;
}				t_parser;

typedef struct s_variable
{
	char	*var;
	char	*val;
	int		exported;
	struct s_variable *next;
}				t_variable;

typedef struct	s_varlist
{
	t_variable			*var_data;
	struct s_varlist	*next;
}				t_varlist;

typedef struct	s_command
{
	char	*cmd;
	char	**args;
	char	*infile;
	char	*outfile;
	int		append;
	int		here_doc;
	char	*limiter;
}				t_command;

typedef struct	s_cmdlist
{
	t_command			*command;
	t_varlist			*var_list;
	struct	s_cmdlist	*next;
}				t_cmdlist;

//readline_utils.c
int		has_unclosed_quote(char *input);
//minishell.c
void	minishell(char *input, t_varlist **head_var, char **envp);
//parsing.c
t_cmdlist	*parse_cmd_line(t_parser *parser, t_varlist **head_var);
//parsing_utils.c
int		ft_strcmp(char *s1, char *s2);
void	skip_whitespace(t_parser *parse);
//var_list.c
void    create_var_list_or_find_node(t_varlist **head, char *input);
int		is_valide_varname(char *input);
//var_list_utils.c
int		if_export_variable(char *content);
void	add_var_lst_front(t_varlist **head, t_varlist *var_node);
int		ft_list_size(t_varlist **head);
void	clean_var_list(t_varlist **head);
//var_control.c
char	*if_variable_var(char *input);
char	*if_variable_val(char *input);
//tokenize_utils.c
int		if_quote(char *str);
//var_val.c
int 	init_registre_variable(t_variable *var_dt, char *input);
//extract_cmd_utils.c
char	*find_words_in_quote(char *content, char ch);
//tokenize.c
t_token	*tokenize(t_parser *parser, t_varlist **head_var);
t_token *create_token(t_tk_type type, char *value);
//tokenize_type.c
t_token	*create_pipe_token(t_parser *parser);
t_token	*create_input_token(t_parser *parser);
t_token	*create_output_token(t_parser *parser);
t_token	*create_and_token(t_parser *parser);
t_token	*create_simple_token(t_parser *parser, int type, const char *s);
//extract_word.c
char	*extract_word(t_parser *parser, t_varlist **head_var);
//dollar_sign.c
char	*reg_dollar_sign(char *str, t_varlist **head_var);
//dollar_sign_utils.c
int		if_dollar_sign(char *str);
int		is_varname_format(char *str);
void	free_vars_vals(char **vars, char **vals);
int		real_length_of_word(char *str, int vals_len);

#endif