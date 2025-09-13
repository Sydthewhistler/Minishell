/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:15:36 by cprot             #+#    #+#             */
/*   Updated: 2025/09/13 09:15:37 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (!result)
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
