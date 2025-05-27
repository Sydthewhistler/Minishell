/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:53:13 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/27 18:17:25 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*str2;
	int		i;

	if (!str || n < 0)
		return (NULL);
	str2 = malloc(sizeof(char) * (n + 1));
	if (!str2)
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		str2[i] = str[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
}

void	free_localvar(t_localvar *localvar)
{
	free(localvar->name);
	free(localvar->value);
	free(localvar);
}