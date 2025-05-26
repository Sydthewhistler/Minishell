/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 14:39:43 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*parse_line(char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (line[i] == '\0')
			break ;
		if (line[i] == '"' || line[i] == '\'')
			i = parse_quoted(line, &i, &tokens);
		else if (line[i] == '\\')
			i = parse_escaped(line, &i, &tokens);
		else if (is_operator(line[i]))
			i = parse_operator(line, &i, &tokens);
		else
			i = parse_word(line, &i, &tokens);
	}
	assign_roles(tokens);
	return (tokens);
}

