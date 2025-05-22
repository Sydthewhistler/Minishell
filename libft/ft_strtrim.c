/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 09:22:24 by cprot             #+#    #+#             */
/*   Updated: 2025/03/31 16:14:54 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	index_start(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	if (!s1 || !set)
		return (0);
	if (set[0] == '\0')
		return (0);
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			return (i);
		i++;
	}
	return (i);
}

unsigned int	index_end(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	j;

	i = (unsigned int)(ft_strlen(s1));
	j = 0;
	if (!s1 || !set || (ft_strlen(s1) == 0))
		return (0);
	if (set[0] == '\0')
		return (ft_strlen(s1));
	while (i > 0)
	{
		i--;
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (set[j] == '\0')
			return (i + 1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	unsigned int	end;
	unsigned int	i;
	char			*new_s;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	start = index_start(s1, set);
	end = index_end(s1, set);
	if (start >= end)
		return (ft_strdup(""));
	i = 0;
	new_s = malloc(sizeof(char) * (end - start + 1));
	if (!new_s)
		return (NULL);
	while (start < end)
	{
		new_s[i] = s1[start];
		i++;
		start++;
	}
	new_s[i] = '\0';
	return (new_s);
}
