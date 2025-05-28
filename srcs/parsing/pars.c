/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/05/28 12:54:16 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_quoted(char *s, int *i, t_token **tokens, char c)
{
	char	temp[1024];
	char	*str;
	int		j;

	j = 0;
	(*i)++;
	while (s[*i] != c)
	{
		if (s[*i] == '\0')
		{
			write(2, "Error : unclosed quotes\n", 25);
			return ;
		}
		temp[j] = s[*i];
		j++;
		(*i)++;
	}
	temp[j] = '\0';
	str = malloc(j + 1);
	if (!str)
		return ;
	ft_strlcpy(str, temp, j + 1);
	create_token(tokens, str, CONTENT_QUOTED, (c == '"'));
	free(str);
}

void	parse_heredoc(char *s, int *i, t_token **tokens)
{
	char	*delimiter;
	char	*heredoc_content;

	*i += 2;
	delimiter = extract_delimiter(s + *i);
	if (!delimiter || !delimiter[0])
	{
		if (delimiter)
			free(delimiter);
		return ;
	}
	heredoc_content = handle_heredoc(delimiter);
	create_token(tokens, "<<", CONTENT_OPERATOR, false);
	create_token(tokens, heredoc_content, CONTENT_HEREDOC, false);
	free(heredoc_content);
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n'
		&& s[*i] != '|' && s[*i] != '<' && s[*i] != '>')
		(*i)++;
	free(delimiter);
	skip_whitespace(s, i);
}

void	parse_operator(char *line, int *i, t_token **tokens)
{
	char	s[2];

	s[0] = line[*i];
	s[1] = '\0';
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		create_token(tokens, ">>", CONTENT_OPERATOR, false);
		(*i) += 2;
	}
	else
	{
		create_token(tokens, s , CONTENT_OPERATOR, false);
		(*i)++;
	}
	skip_whitespace(line, i);
}

// void	parse_var(char *line, int *i, t_token **tokens)
// {
	
// }

void	parse_line(char *line, t_token **tokens)
{
	int	i;

	i = 0;
	skip_whitespace(line, &i);
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'') // Quotes (priorité haute)
		{
			parse_quoted(line, &i, tokens, line[i]);
			i++;
			skip_whitespace(line, &i);
		}
		else if (line[i] == '<' && line[i + 1] == '<') // heredoc
			parse_heredoc(line, &i, tokens);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			parse_operator(line, &i, tokens);
		else if (line[i] == '$') // Variables GERER LE ' " attention"
			parse_var(line, &i, tokens);
		// else // Mots normaux (priorité basse)
		// {
		// 	// Gérer les mots normaux
		// 	i++;
		// }
		else
			return ;
	}
}
// complete_line = complete_input(line);
// if (!complete_line)
// 	return (NULL);