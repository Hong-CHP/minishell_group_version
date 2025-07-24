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
    t_tk_type type;
    char *value;
    struct s_token *next;
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

//minishell.c
void	minishell(char *input, t_varlist **head_var, char **envp);
//parsing_utils.c
int		ft_strcmp(char *s1, char *s2);
//var_list.c
void    create_var_list_or_find_node(t_varlist **head, char *input);
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

#endif