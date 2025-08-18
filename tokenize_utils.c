#include "minishell.h"
#include "libft.h"

int		if_quote(char *str)
{
	int		i;
	char	ch;

	i = 0;
	printf("str os %s\n", str);
	while (str[i] && str[i] != '\''&& str[i] != '"')
		i++;
	printf("str[i] is %c\n", str[i]);
	if (str[i] == '\'' || str[i] == '"')
	{
		ch = str[i];
		i++;
	}
	printf("ch is %c\n", ch);
	while (str[i] && str[i] != ch)
		i++;
	if (str[i] == ch)
		return ((int)ch);
	return (0);
}

void	free_token_list(t_token **head)
{
	t_token	*cur;
	t_token	*next;

	cur = *head;
	while (cur && cur->next)
	{
		next = cur->next;
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = next;
	}
	*head = NULL;
}