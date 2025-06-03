/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substrlen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:42:53 by cprot             #+#    #+#             */
/*   Updated: 2025/06/03 10:43:19 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr_len(char const *s, unsigned int start, unsigned int len)
{
	char *str;
	size_t i;

	if (!s || start >= ft_strlen(s))
		return (ft_strdup(""));

	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;

	str = malloc(len + 1);
	if (!str)
		return (NULL);

	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
