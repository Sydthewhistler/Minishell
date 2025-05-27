/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/05/27 14:17:38 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_strings(char *s1, char *s2)
{
	char	*result;
	char	*temp;

	temp = ft_strjoin(s1, s2);
	result = ft_strjoin(temp, "\\n");
	free(s1);
	free(temp);
	return (result);
}

char	*handle_quotes(char *s, char *prompt)
{
	int		i;
	char	*result;
	char	*temp;
	
	i = 1;
	while (s[i] && s[i] != s[0])
		i++;
	if (s[i] == s[0])
		return (ft_strdup(s));
	result = ft_strdup(s);
	while (1)
	{
		temp = readline(prompt);
		if (!temp)
			break ;
		result = join_strings(result, temp);
		if (ft_strrchr(temp, s[0]))
		{
			free(temp);
			break ;
		}
		free(temp);
	}
	return (result);
}

// char	*handle_heredoc(char *s)
// {

// }

char	*complete_input(char *line)
{
	char	*result;

	// char	*final_result;
	if (!line)
		return (NULL);
	if (line[0] == '"')
		result = handle_quotes(line, "dquote> ");
	else if (line[0] == '\'')
		result = handle_quotes(line, "quote> ");
	else
		result = ft_strdup(line);
	if (!result)
		return (NULL);
	// final_result = handle_heredoc(result);
	// if (final_result && final_result != result)
	// 	free(result);
	printf("string = %s\n", result);
	return (result);
}

t_token	*parse_line(char *line)
{
	char	*complete_line;

	// t_token	*tokens;
	complete_line = complete_input(line);
	// tokens = tokenize(complete_line);
	// assign_roles(tokens);
	// free(complete_line);
	return (NULL);
}
