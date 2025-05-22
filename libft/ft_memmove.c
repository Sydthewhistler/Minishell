/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 09:11:33 by cprot             #+#    #+#             */
/*   Updated: 2025/03/31 09:11:35 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*new_dest;
	const unsigned char	*new_src;
	size_t				i;

	new_dest = (unsigned char *) dest;
	new_src = (const unsigned char *) src;
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			new_dest[i] = new_src[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i > 0)
		{
			new_dest[i - 1] = new_src[i - 1];
			i--;
		}
	}
	return (dest);
}
