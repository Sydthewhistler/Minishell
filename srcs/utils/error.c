/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:04:18 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/04 17:49:28 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	error(char *str)
{
	perror(str);
	exit(-1);
}

void	putstr_error(char *str)
{
	int i;

	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}