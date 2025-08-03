#include "minishell.h"
#include "libft.h"

int	if_slash(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			count++;
		i++;
	}
	return (count);
}