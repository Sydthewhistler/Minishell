/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:04:18 by scavalli          #+#    #+#             */
/*   Updated: 2025/08/12 20:38:55 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <minishell.h>

void	putstr_error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}

void	ft_error_cmd_not_found(char *cmd)
{
	printf("%s: command not found\n", cmd);
	g_signal = COMMAND_NOT_FOUND;
}

void	ft_error_permission_denied(char *cmd)
{
	printf("minishell: %s: Permission denied\n", cmd);
	g_signal = PERMISSION_DENIED;
}

void	ft_error_file(char *cmd, int is_dir)
{
	if (is_dir)
	{
		printf("minishell: %s: Is a directory\n", cmd);
		g_signal = PERMISSION_DENIED;
	}
	else
	{
		printf("minishell: %s: No such file or directory\n", cmd);
		g_signal = COMMAND_NOT_FOUND;
	}
}

void	ft_error_syntax(char *token)
{
	if (token)
		printf("minishell: syntax error near unexpected token '%s'\n", token);
	else
		printf("minishell: syntax error near unexpected token 'newline'\n");
	g_signal = SYNTAX_ERROR_CODE;
}
