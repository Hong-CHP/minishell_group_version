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

void	init_varnode_vardata_for_ev(t_varlist **var_node)
{
	*var_node = malloc(sizeof(t_varlist));
	if (!*var_node)
		return ;
	ft_memset(*var_node, 0, sizeof(t_varlist));
	(*var_node)->var_data = malloc(sizeof(t_variable));
	if (!(*var_node)->var_data)
	{
		free((*var_node));
		return ;
	}
	ft_memset((*var_node)->var_data, 0, sizeof(t_variable));
	(*var_node)->var_data->exported = 1;
}

void	create_var_list_for_ev(t_varlist **head, char *input)
{
	t_varlist	*var_node;
	int			len;
	int			i;
	
	init_varnode_vardata_for_ev(&var_node);
	var_node->var_data->var = if_variable_var(input);
	if (!var_node->var_data->var)
		return ;
	i = 0;
	len = 0;
	while (input[i] && input[i] != '=')
		i++;
	while (input[i])
	{
		len++;
		i++;
	}
	var_node->var_data->val = malloc(sizeof(char) * (len + 1));
	if (!var_node->var_data->val)
		return ;
	fill_variable_value(input, var_node->var_data->var, var_node->var_data->val);
	var_node->next = NULL;
	add_var_lst_order(head, var_node);
}
