#include "minishell.h"

static char	*finalize_heredoc_result(char *result)
{
	char	*final_result;

	if (result && result[0] != '\0')
	{
		final_result = ft_strjoin(result, "\n");
		free(result);
		return (final_result);
	}
	return (result);
}

char	*handle_heredoc(char *delimiter)
{
	char	*result;
	char	*temp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (1)
	{
		temp = readline("heredoc> ");
		if (!temp) // Handle EOF (Ctrl+D)
		{
			write(1, "\n", 1);
			break ;
		}
		if (ft_strcmp(temp, delimiter) == 0)
		{
			free(temp);
			break ;
		}
		result = add_line_to_result(result, temp);
		free(temp);
		if (!result) // Check for allocation failure
			return (NULL);
	}
	return (finalize_heredoc_result(result));
}

char	*build_heredoc_history(char *line, char *content, char *delimiter)
{
	char	*history;
	char	*temp;

	temp = ft_strjoin(line, "\n");
	history = ft_strjoin(temp, content);
	free(temp);
	temp = history;
	history = ft_strjoin(temp, "\n");
	free(temp);
	temp = history;
	history = ft_strjoin(temp, delimiter);
	free(temp);
	return (history);
}

void	update_history_entry(char *line, char *content, char *delimiter)
{
	char *history_entry;
	HIST_ENTRY *last_entry;

	history_entry = build_heredoc_history(line, content, delimiter);
	last_entry = remove_history(history_length - 1);
	if (last_entry)
	{
		free(last_entry->line);
		free(last_entry);
	}
	add_history(history_entry);
	free(history_entry);
}