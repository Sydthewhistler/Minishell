/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/05/22 14:35:56 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*pars_line(char *line)
{
	t_list	*tokens;

	tokens = ft_split_token(line);
	while (!*tokens)
	{
		tokens->type = find_type(*tokens);
	}
}

/*recup *line
token = ft_split line
init_struct :
	dans une boucle while token[i]
	token[i] = struct.str;
	find_type = struct.type;
find_type ...
*/