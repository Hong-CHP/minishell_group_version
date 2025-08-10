#include "minishell.h"
#include "libft.h"

int		count_size_of_envp(char **envp)
{
	int	size;

	size = 0;
	while(envp[size])
		size++;
	return (size);
}

void	print_all_variable_in_list(t_varlist **head)
{
	t_varlist	*cur;

	cur = *head;
	while (cur)
	{
		if (cur->var_data->exported == 1)
			printf("declare -x %s=\"%s\"\n", cur->var_data->var, cur->var_data->val);
		cur = cur->next;
	}
}

void	set_varlist_exported(t_varlist **head_var)
{
	t_varlist	*cur;

	cur = *head_var;
	while (cur)
	{
		cur->var_data->exported = 1;
		cur = cur->next;
	}
}