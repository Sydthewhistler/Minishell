/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:12:38 by cprot             #+#    #+#             */
/*   Updated: 2025/06/04 18:35:08 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
}

t_token	*found_last(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

void	create_operator_token(t_token **tokens, char *operator,
		int content_type)
{
	create_token(tokens, operator, content_type);
}

void	create_token(t_token **tokens, char *content, int content_type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token || !tokens)
		return ;
	new_token->str = ft_strdup(content);
	if (!new_token->str)
	{
		free(new_token);
		return ;
	}
	new_token->type = content_type;
	new_token->role = 0;
	new_token->next = NULL;
	new_token->prev = NULL;
	if (!(*tokens))
		*tokens = new_token;
	else
	{
		last = found_last(*tokens);
		last->next = new_token;
		new_token->prev = last;
	}
}

