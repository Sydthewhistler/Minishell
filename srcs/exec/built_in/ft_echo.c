/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:18:35 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/24 10:22:07 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include "exec.h"

void	ft_echo(t_token *lst)
{
	if(!lst->next) // si rien apres juste \n
	{
		printf("\n");
		return ;
	}
	else if(lst->next->role != ROLE_ARGUMENT) // si qqlch apres mais pas argument "ex : echo | ls" rien faire
		return ;
	else
	{
		if(!strcmp(lst->next->str, "-n")) // si -n
		{
			printf("%s\x1b[7m%%\x1b[0m\n", lst->next->next->str); //code ANSI % surligné
			return ;
		}
		printf("%s\n", lst->next->str);
	}
}
