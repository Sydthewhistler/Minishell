/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:43 by scavalli          #+#    #+#             */
/*   Updated: 2025/08/17 16:33:42 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

void	ft_pwd(t_env *env, t_token *token)
{
	char	*pwd;

	(void)token;
	pwd = return_env_value("PWD", env);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("pwd");
			return ;
		}
		printf("%s\n", pwd);
		free(pwd);
		return ;
	}

	printf("%s\n", pwd);
}
