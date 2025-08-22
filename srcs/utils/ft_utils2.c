#include "exec.h"
#include "minishell.h"

char	*ft_filename(t_token *token)
{
	while (token->role != ROLE_FILENAME)
		token = token->next;
	return (token->str);
}

char	*extract_delimiter(char *s)
{
	int		i;
	int		start;
	int		len;
	char	*delimiter;

	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	start = i;
	while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
		i++;
	len = i - start;
	delimiter = malloc(sizeof(char) * (len + 1));
	if (!delimiter)
		return (NULL);
	i = 0;
	while (i < len)
	{
		delimiter[i] = s[start + i];
		i++;
	}
	delimiter[len] = '\0';
	return (delimiter);
}

char	*add_line_to_result(char *result, char *line)
{
	char	*new_result;
	char	*temp;

	if (result && result[0] != '\0')
	{
		temp = ft_strjoin(result, "\n");
		free(result);
		if (!temp)
			return (NULL);
		result = temp;
	}
	new_result = ft_strjoin(result, line);
	if (result)
		free(result);
	return (new_result);
}
