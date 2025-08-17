#include "minishell.h"

void	create_and_advance(t_token **tokens, char *op, int *i, int len)
{
	create_token(tokens, op, CONTENT_OPERATOR);
	(*i) += len;
}

char	*append_char(char *str, char c)
{
	int		len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	ft_strcpy(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

static char	*handle_variable_cases(char *line, int *i, t_shell *shell)
{
	int		start;
	char	*name;
	char	*value;

	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_code));
	}
	if (ft_isdigit(line[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (!ft_isalpha(line[*i]) && line[*i] != '_')
		return (ft_strdup("$"));
	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	name = ft_substr_len(line, start, *i - start);
	value = get_var_value(shell->env, *(shell->localvar), name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

char	*append_variable_content(char *result, char *line, int *i,
		t_shell *shell)
{
	char	*var_value;
	char	*new_result;

	(*i)++;
	var_value = handle_variable_cases(line, i, shell);
	new_result = ft_strjoin(result, var_value);
	free(result);
	free(var_value);
	return (new_result);
}

static int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>'
		|| c == '&');
}

char	*parse_complete_segment(char *line, int *i, t_shell *shell)
{
	char *result;

	result = ft_strdup("");
	while (line[*i] && !is_separator(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
			result = append_quoted_content(result, line, i, shell);
		else if (line[*i] == '$')
			result = append_variable_content(result, line, i, shell);
		else
			result = append_char(result, line[(*i)++]);
	}
	return (result);
}