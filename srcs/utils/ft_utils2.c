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

char *add_line_to_result(char *result, char *line)
{
	char *new_result;
	char *temp;

	if (!result || !line)
		return (NULL);
	// If result is not empty, we need to add a newline before the new line
	if (result[0] != '\0')
	{
		temp = ft_strjoin(result, "\n");
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		new_result = ft_strjoin(temp, line);
		free(temp);
		free(result);
	}
	else
	{
		// If result is empty, just copy the line
		new_result = ft_strjoin(result, line);
		free(result);
	}
	return (new_result);
}

void	check_exit_code(t_token *tokens, t_shell *shell)
{
	while(tokens)
	{
		if(tokens->exit_code != -2)
			shell->exit_code = tokens->exit_code;
		tokens = tokens->next;
	}
}

bool	check_error(t_token *tokens)
{
	while(tokens)
	{
		if(tokens->exit_code != -2 && tokens->exit_code != 0)
			return (true);
		tokens = tokens->next;
	}
	return (false);
}