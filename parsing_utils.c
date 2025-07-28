#include "minishell.h"
#include "libft.h"

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

void skip_whitespace(t_parser *parse)
{
	while (parse->input[parse->pos] && 
		(parse->input[parse->pos] == ' ' || 
		 parse->input[parse->pos] == '\t' || 
		 parse->input[parse->pos] == '\n'))
		parse->pos++;
}

