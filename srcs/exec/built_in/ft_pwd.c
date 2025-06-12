/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:43 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/12 15:50:09 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_env *env)
{
	char	*pwd;

	pwd = return_env_value("PWD", env); // va chercher valeur de PWD dans t_env
	printf(pwd);
	return ;
}