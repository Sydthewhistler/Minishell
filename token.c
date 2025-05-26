/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:00:47 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 13:59:56 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_split_token(char *s)
{
	int		i;
	int		j;
	char	c;
	char	*str;
	char	temp[1024];
	t_list	*list;

	i = 0;
	j = 0;
	list = NULL;
	if (!str)
		return NULL;
	if (s[i] == '"' || s[i] == '\'')
	{
		c = s[i];
		i++;
		while (s[i] != c)
		{
			temp[j++] = s[i++];
			if (s[i] == '\0')
			{
				if (c = '"')
					s = readline("dquote>");
				else
					s = readline("quote>");
			}
			i = 0;
		}
		temp[j] = '\0';
		str = malloc(j + 1);
		ft_strcpy(str, temp);
		create_node(&list, str, 0);
	}
	return (list);
}
