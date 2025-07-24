#include "minishell.h"
#include "libft.h"

int	if_export_variable(char *content)
{
	if (ft_strncmp("export ", content, 7) == 0)
		return (1);
	return (0);
}

int ft_list_size(t_varlist **head)
{
    int count;
	t_varlist	*cur;
    
    count = 0;
    cur = *head;
    while (cur)
    {
        count++;
        cur = cur->next;
    }
    return (count);
}

void	clean_var_list(t_varlist **head)
{
    t_varlist   *cur;
    t_varlist   *next;
    
    cur = *head;
    while (cur)
    {
        next = cur->next;
        free(cur->var_data);
        free(cur);
        cur = next;
    }
    *head = NULL;
}

void	add_var_lst_front(t_varlist **head, t_varlist *var_node)
{
	if (*head && var_node)
		var_node->next = *head;
	*head = var_node;
}