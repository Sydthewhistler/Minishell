/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:18:35 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/26 18:09:55 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_echo(char *cmd, t_token *lst)
{
	int fd;
	int i;

	i = 0;
	if(!lst->next)
	{
		printf("\n");
		return ;
	}
	if(lst->next->next && lst->next->next->role == ROLE_REDIRECT_OUT) // regarder si output dans fichier
	{
		fd = open(lst->next->next->next->s, O_WRONLY | O_CREAT, 0644);
		if(fd == -1)
			error("Error while opening echo output fd");
		ft_putstr_fd(lst->next->s, fd);
		if(!ft_strchr(lst->s, "-n"))
			write(fd,'\n',1);
		close(fd);
	}
	else
	{
		printf("%s", lst->next->s);
		if(!ft_strchr(lst->s, "-n"))
			printf("\n");
	}
}
