/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 17:37:39 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*complete_input(char *line)
{
	char	*result;

	result = ft_strdup(line);
	result = handle_quotes(result);
	result = handle_heredoc(result);
	return (result);
}

t_token	*parse_line(char *line)
{
	char	*complete_line;
	t_token	*tokens;

	complete_line = complete_input(line);
	tokens = tokenize(complete_line);
	assign_roles(tokens);
	free(complete_line);
	return (tokens);
}

void	parse_quoted(char *s, int *i, t_token **tokens)
{
	char c;
	char temp[1024];
	char *str;
	int j;

	c = s[*i];
	j = 0;
	(*i)++;
	while (s[*i] != c && s[*i] != '\0')
		temp[j++] = s[(*i)++];
	(*i)++;
	temp[j] = '\0';
	str = malloc(j + 1);
	if (!str)
		return ;
	ft_strlcpy(str, temp, j + 1);
	create_token(tokens, str, CONTENT_QUOTED);
}
