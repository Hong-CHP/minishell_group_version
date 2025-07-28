#include "minishell.h"
#include "libft.h"

void	free_vars_vals(char **vars, char **vals)
{
	int i;

	i = 0;
	while (vars[i])
	{
		free(vars[i]);
		if (vals[i])
			free(vals[i]);
		i++;
	}
	free(vars);
	vars = NULL;
	if (vals)
		free(vals);
	vals = NULL;
}

int	real_length_of_word(char *str, int vals_len)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while(str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i++;
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '{')
				i++;
			while(ft_isalnum(str[i]) || str[i] == '_')
				i++;
			if (str[i] == '}')
				i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	len = len + vals_len;
	return (len);
}

int if_dollar_sign(char *str)
{
	int i;
	int count;
		
	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

int	is_varname_format_brackets(char *str, int i)
{
	i++;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != '}')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '}')
		return (1);
	else
		return (0);
}

int	is_varname_format(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '{')
		return (is_varname_format_brackets(str, i));
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && str[i] != ' ' && str[i] != ',' && str[i] != '$')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
