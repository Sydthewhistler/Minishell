/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:36:56 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/26 17:37:12 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *line)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	start;
	char			*path;

	i = 0;
	j = 0;
	while (line[i] && line[i] != ' ')
		i++;
	start = i + 1;
	while (line[i])
	{
		j++;
		i++;
	}
	path = ft_substr(line, start);
	chdir(path);
	free(path);
}