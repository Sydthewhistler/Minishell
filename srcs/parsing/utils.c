/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:36:53 by cprot             #+#    #+#             */
/*   Updated: 2025/05/28 12:47:15 by cprot            ###   ########.fr       */
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
	create_token(tokens, operator, content_type, false);
}

void	create_token(t_token **tokens, char *content, int content_type,
		bool need_expansion)
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
	new_token->need_expansion = need_expansion;
	if (!(*tokens))
		*tokens = new_token;
	else
	{
		last = found_last(*tokens);
		last->next = new_token;
		new_token->prev = last;
	}
}
