#include "minishell.h"
#include "libft.h"

void	fill_variable_value(char *content, char *var, char *val)
{
	int	i;
	int	j;

	i = 0;
	while (content[i] && content[i] != '=')
	{
		var[i] = content[i];
		i++;
	}
	var[i] = '\0';
	if (content[i] && content[i] == '=')
		i++;
	j = 0;
	while (content[i])
	{
		val[j] = content[i];
		j++;
		i++;
	}
	val[j] = '\0';
}

char	*extract_value(char *val)
{
	char	*value;
	int		ch;

	if (if_quote(val) != 0 && val[0] != val[ft_strlen(val) - 1])
		return (NULL);
	ch = if_quote(val);
	value = find_words_in_quote(val, ch);
	if (!value)
		return (NULL);
	return (value);
}

t_variable	*registre_var_val(char *input, t_variable *var_dt, char *value)
{
	if (var_dt->exported == 1)
		fill_variable_value(&input[7], var_dt->var, var_dt->val);
	else
		fill_variable_value(input, var_dt->var, var_dt->val);
	if (if_quote(var_dt->val) != 0)
	{
		value = extract_value(var_dt->val);
		if (!value)
		{
			free(var_dt->val);
			return (NULL);
		}	
		free(var_dt->val);
		var_dt->val = value;
	}
	return (var_dt);
}

t_variable	*verify_and_init_var_val(char *input, t_variable *var_dt)
{
	if (var_dt->exported == 1)
		var_dt->var = if_variable_var(&input[7]);
	else
		var_dt->var = if_variable_var(input);
	if (!var_dt->var)
		return (NULL);
	var_dt->val = if_variable_val(input);
	if (!var_dt->val)
	{
		free(var_dt->var);
		return (NULL);
	}
	return (var_dt);
}

int 	init_registre_variable(t_variable *var_dt, char *input)
{
	char	*value;

	value = NULL;
	if (if_export_variable(input))
		var_dt->exported = 1;
	if (!verify_and_init_var_val(input, var_dt))
		return (0);
	if (!registre_var_val(input, var_dt, value))
		return (0);
	return (1);
}
