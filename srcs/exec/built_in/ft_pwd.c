/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:43 by scavalli          #+#    #+#             */
/*   Updated: 2025/07/05 15:08:26 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	ft_pwd(t_env *env, t_token *token)
{
	char	*pwd;

	if(token->next && (token->next->role != ROLE_PIPE && token->next->role != ROLE_REDIRECT_OUT))
	{
		printf("pwd: too many arguments\n");
		return ;
	}
	pwd = return_env_value("PWD", env); // va chercher valeur de PWD dans t_env
	printf("%s\n", pwd);
	return ;
}