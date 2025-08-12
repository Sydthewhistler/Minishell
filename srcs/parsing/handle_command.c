/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:53:50 by cprot             #+#    #+#             */
/*   Updated: 2025/08/06 23:54:03 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Vérifie si un chemin est valide et exécutable
static int	check_command_errors(char *envp, char *str)
{
	struct stat	info;

	if (stat(envp, &info) != 0)
		return (ft_error_file(str, 0), 0);
	if (S_ISDIR(info.st_mode))
		return (ft_error_file(str, 1), 0);
	if (access(envp, X_OK) != 0)
		return (ft_error_permission_denied(str), 0);
	return (1);
}

static int	handle_operator_error(t_token *current)
{
	if (current->str && current->str[0] == '|')
	{
		if (current->str[1] == '|')
			ft_error_syntax("||");
		else
			ft_error_syntax("|");
	}
	else if (current->str && current->str[0] == '>')
	{
		if (current->str[1] == '>')
			ft_error_syntax("newline"); // >> -> newline
		else
			ft_error_syntax("newline"); // > -> newline
	}
	else if (current->str && current->str[0] == '<')
	{
		if (current->str[1] == '<')
			ft_error_syntax("newline"); // << -> newline
		else
			ft_error_syntax("newline"); // < -> newline
	}
	else if (current->str && current->str[0] == '&')
	{
		if (current->next && current->next->str && current->next->str[0] == '&')
			ft_error_syntax("&&");
		else
			ft_error_syntax("&");
	}
	else
	{
		if (current->next && current->next->str
			&& current->next->str[0] != '\0')
			ft_error_syntax(current->next->str);
		else
			ft_error_syntax("newline"); // Par défaut -> newline
	}
	return (0);
}

// Traite les commandes externes (non-builtins)
static int	handle_external_command(t_token *current, t_env *env)
{
	current->envp = search_path(current->str, env);
	if (!current->envp)
		return (ft_error_cmd_not_found(current->str), 0);
	if (!check_command_errors(current->envp, current->str))
		return (0);
	return (1);
}

// Point d'entrée pour traiter une commande
int	handle_command_state(t_token *current, t_parser_state *state, t_env *env)
{
	if (!current || !state || !env)
		return (0);
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		if (!current->str || ft_strlen(current->str) == 0)
			return (1);
		if (ft_strrchr(current->str, ' '))
			return (ft_error_cmd_not_found(current->str), 0);
		current->role = ROLE_COMMAND;
		if (is_builtin(current))
			current->envp = NULL;
		else if (!handle_external_command(current, env))
			return (0);
		*state = EXP_ARG;
		return (1);
	}
	else if (current->type == CONTENT_OPERATOR)
		return (handle_operator_error(current));
	return (1);
}
