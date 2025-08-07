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

void	print_all_variable_in_list(t_varlist **head, char **envp)
{
	t_varlist	*cur;
	size_t		export_val_len;

	cur = *head;
	while (cur)
	{
		cur->var_data->exported = 1;
		export_val_len = 11 + ft_strlen(cur->var_data->var);
		cur = cur->next;
	}
}

void	export_vars(t_varlist **head, char **envp)
{
	int		envp_size;
	int		i;
	t_varlist	*cur;

	envp_size = count_size_of_envp(envp);
	i = 0;
	while (i < envp_size)
	{
		create_var_list(head, envp[i]);
		i++;
	}
	cur = *head;
	while (cur)
	{
		int j = 0;
		while (envp[j])
		{
			if (ft_strnstr(envp[j], cur->var_data->var, ft_strlen(cur->var_data->var))
				&& ft_strcmp(cur->var_data->val, &envp[j][ft_strlen(envp[j]) + 1]))
				cur->var_data->exported = 1;
			j++;
		}
		cur = cur->next;
	}
	print_all_variable_in_list(head, envp);
}