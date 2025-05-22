/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 09:19:42 by cprot             #+#    #+#             */
/*   Updated: 2025/03/31 09:19:44 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_c(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

void	putnbr_dest(char *dest, int n, int len)
{
	if (n >= 10)
		putnbr_dest(dest, n / 10, len - 1);
	dest[len] = (n % 10) + '0';
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*int_min(void)
{
	char	*dest;

	dest = malloc(12 * sizeof(char));
	if (!dest)
		return (NULL);
	return (ft_strcpy(dest, "-2147483648"));
}

char	*ft_itoa(int n)
{
	char	*dest;
	int		len;
	int		neg;

	len = 0;
	neg = 0;
	if (n == -2147483648)
		return (int_min());
	if (n < 0)
	{
		n = -n;
		len++;
		neg = 1;
	}
	len = len + count_c(n);
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	dest[len] = '\0';
	if (neg)
		dest[0] = '-';
	putnbr_dest(dest, n, len - 1);
	return (dest);
}
