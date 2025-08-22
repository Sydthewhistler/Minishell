#include "minishell.h"

static char	*process_quote_expansion(char *content, char quote, t_shell *shell)
{
	char	*expanded;

	if (quote == '"')
	{
		expanded = handle_expand_in_quotes(content, shell);
		free(content);
		return (expanded);
	}
	return (content);
}

char	*extract_quoted_content(char *line, int *i, t_shell *shell)
{
	char	quote;
	char	*content;
	int		start;

	quote = line[*i];
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (!line[*i])
	{
		write(2, "Error: unclosed quotes\n", 24);
		return (ft_strdup(""));
	}
	content = ft_substr_len(line, start, *i - start);
	(*i)++;
	return (process_quote_expansion(content, quote, shell));
}

char	*append_quoted_content(char *result, char *line, int *i, t_shell *shell)
{
	char	*quoted_content;
	char	*new_result;

	quoted_content = extract_quoted_content(line, i, shell);
	new_result = ft_strjoin(result, quoted_content);
	free(result);
	free(quoted_content);
	return (new_result);
}
