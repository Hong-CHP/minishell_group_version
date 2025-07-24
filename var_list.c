#include "minishell.h"
#include "libft.h"

t_variable	*search_target_var_node(t_varlist **head, char *find_var)
{
	t_varlist	*cur;

	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->var_data->var, find_var) == 0)
			return (cur->var_data);
		cur = cur->next;
	}
	return (NULL);
}

void    find_var_node_modif_val(t_variable *modif_var_node, char *match)
{
    char *find_val;

    find_val = ft_strdup(match + 1);
    if (!find_val)
        return ;
    if (modif_var_node->val)
        free(modif_var_node->val);
    modif_var_node->val = find_val;
}

void	create_var_list(t_varlist **head, char *input)
{
	t_varlist	*var_node;

	var_node = malloc(sizeof(t_varlist));
	if (!var_node)
		return ;
	var_node->var_data = malloc(sizeof(t_variable));
	if (!var_node->var_data)
	{
		free(var_node);
		return ;
	}
	ft_memset(var_node->var_data, 0, sizeof(t_variable));
	if (!init_registre_variable(var_node->var_data, input))
	{
		free(var_node->var_data);
		free(var_node);
		return ;
	}
	var_node->next = NULL;
	add_var_lst_front(head, var_node);
}

void	process_var_val_export(t_varlist **head, char *input, t_variable *var_node)
{
	char *match_var;
	char   *find_var;
		
	if (ft_strchr(input, '='))
	{
		match_var = ft_strchr(input, '=');
		find_var = ft_substr(input, 7, match_var - (input + 7));
	}
	else
		find_var = ft_substr(input, 7, ft_strlen(input) - 7);
	var_node = search_target_var_node(head, find_var);
	if (!var_node)
		create_var_list(head, input);
	else
	{
		if (ft_strchr(input, '='))
			find_var_node_modif_val(var_node, match_var);
		var_node->exported = 1;
	}
	if (find_var)
		free(find_var);
}

void	create_var_list_or_find_node(t_varlist **head, char *input)
{
	char		*match_var;
	char		*find_var;
	t_variable  *var_node;

	var_node = NULL;
	match_var = NULL;
	find_var = NULL;
	if (if_export_variable(input))
		process_var_val_export(head, input, var_node);
	else if (!if_export_variable(input) && ft_strchr(input, '='))
	{
		if (ft_list_size(head) > 0)
		{
			match_var = ft_strchr(input, '=');
			find_var = ft_substr(input, 0, match_var - input);
			var_node = search_target_var_node(head, find_var);
			if (!var_node)
				create_var_list(head, input);
			else
				find_var_node_modif_val(var_node, match_var);
			free(find_var);
		}
		else
			create_var_list(head, input);
	}
}
