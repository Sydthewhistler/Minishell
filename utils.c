/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:36:53 by cprot             #+#    #+#             */
/*   Updated: 2025/05/22 14:43:27 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_node(t_list **stack, char *token_str, int token_type)
{
	t_list	*new_node;
	t_list	*last;

	if (!stack)
		return ;
	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_node->next = NULL;
	new_node->str = token_str;
	new_node->type = token_type;
	if (!(*stack))
	{
		*stack = new_node;
		new_node->prev = NULL;
	}
	else
	{
		last = found_last(*stack);
		last->next = new_node;
		new_node->prev = last;
	}
}
