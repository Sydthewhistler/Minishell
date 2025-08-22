#include "minishell.h"

static char	*process_heredoc_line(char *result, char *temp, char *delimiter)
{
	if (!temp)
	{
		write(1, "\n", 1);
		return (result);
	}
	if (ft_strcmp(temp, delimiter) == 0)
	{
		free(temp);
		return (result);
	}
	result = add_line_to_result(result, temp);
	free(temp);
	return (NULL);
}

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
	char	*processed;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (1)
	{
		temp = readline("heredoc> ");
		processed = process_heredoc_line(result, temp, delimiter);
		if (processed)
		{
			result = processed;
			if (temp == NULL || ft_strcmp(temp, delimiter) == 0)
				break ;
		}
		else
			result = temp;
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
	char		*history_entry;
	HIST_ENTRY	*last_entry;

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
