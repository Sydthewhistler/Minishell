/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:36:53 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 18:29:27 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*found_last(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

static void	create_token(t_token **tokens, char *content, int content_type)
{
	t_token	*new_token;
	t_token	*last;

	if (!tokens)
		return ;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->next = NULL;
	new_token->str = content;
	new_token->type = content_type;
	new_token->role = 0;
	if (!(*tokens))
	{
		*tokens = new_token;
		new_token->prev = NULL;
	}
	else
	{
		last = found_last(*tokens);
		last->next = new_token;
		new_token->prev = last;
	}
}
