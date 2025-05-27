/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:00:47 by cprot             #+#    #+#             */
/*   Updated: 2025/05/27 14:20:13 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_quoted(char *s, int *i, t_token **tokens)
{
	char c;
	char temp[1024];
	char *str;
	int j;

	c = s[*i];
	j = 0;
	(*i)++;
	while (s[*i] != c && s[*i] != '\0')
		temp[j++] = s[(*i)++];
	(*i)++;
	temp[j] = '\0';
	str = malloc(j + 1);
	if (!str)
		return ;
	ft_strlcpy(str, temp, j + 1);
	create_token(tokens, str, CONTENT_QUOTED);
}
