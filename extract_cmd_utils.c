#include "minishell.h"
#include "libft.h"

char	*find_words_in_quote(char *content, char ch)
{
	int		i;
	int		start;
    size_t  len;
	char	*str;
	//malloc need to be free then
	
    len = ft_strlen(content);
	i = 0;
	str = NULL;
	start = 0;
	while (i < (int)len && content[i] != ch)
		i++;
	if (content[i] == '\0')
		return (NULL);
	i++;
	start = i;
	while (i < (int)len && content[i] != ch)
		i++;
	if(content[i] == '\0')
		return (NULL);
	str = malloc(sizeof(char) * (i - start + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, &(content[start]), i - start + 1);
	return (str);
}