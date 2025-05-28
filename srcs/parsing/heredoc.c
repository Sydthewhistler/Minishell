/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:56:44 by cprot             #+#    #+#             */
/*   Updated: 2025/05/28 10:19:30 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_line_to_result(char *result, char *line)
{
	char	*new_result;

	if (result[0] != '\0')
	{
		new_result = ft_strjoin(result, "\n");
		free(result);
		result = new_result;
	}
	new_result = ft_strjoin(result, line);
	free(result);
	return (new_result);
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
		if (!temp)
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
	}
	return (result);
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

