#include "minishell.h"
#include "libft.h"

int		if_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return ((int)'\'');
		else if (str[i] == '\"')
			return ((int)'\"');
		i++;
	}
	return (0);
}