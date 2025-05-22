/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:00:47 by cprot             #+#    #+#             */
/*   Updated: 2025/05/22 14:46:52 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_split_token(char *s)
{
	int		i;
	int		j;
	char	c;
	char	*str;
	t_list	*list;

	i = 0;
	j = 0;
	str = malloc(sizeof(*char));
	if (!str)
	{
		return ;
	}
	if (s[i] == '"' || s[i] == '\'')
	{
		c = s[i];
		i++;
		while (s[i] != c)
		{
			str[j++] = s[i++];
		}
		create_node(&list, str, 0); 
	}
}
