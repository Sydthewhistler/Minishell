#include "minishell.h"

// Vérifie si un chemin est valide et exécutable
static int	check_command_errors(char *envp, char *str, t_shell *shell)
{
	struct stat	info;

	if (stat(envp, &info) != 0)
	{
		ft_error_file(str, 0, shell); // ← CORRIGÉ : ajout shell
		return (0);
	}
	if (S_ISDIR(info.st_mode))
	{
		ft_error_file(str, 1, shell); // ← CORRIGÉ : ajout shell
		return (0);
	}
	if (access(envp, X_OK) != 0)
	{
		ft_error_permission_denied(str, shell); // ← CORRIGÉ : ajout shell
		return (0);
	}
	return (1);
}

static int	handle_operator_error(t_token *current, t_shell *shell)
{
	if (current->str && current->str[0] == '|')
	{
		if (current->str[1] == '|')
			ft_error_syntax("||", shell); // ← CORRIGÉ : ajout shell
		else
			ft_error_syntax("|", shell); // ← CORRIGÉ : ajout shell
	}
	else if (current->str && (current->str[0] == '>' || current->str[0] == '<'))
	{
		if (!current->next || !current->next->str
			|| current->next->str[0] == '\0')
			ft_error_syntax("newline", shell); // ← CORRIGÉ : ajout shell
		else if (current->next->type == CONTENT_OPERATOR)
			ft_error_syntax(current->next->str, shell);
				// ← CORRIGÉ : ajout shell
		else
			return (1); // Redirection valide
	}
	else if (current->str && current->str[0] == '&')
	{
		if (current->str[1] == '&')
			ft_error_syntax("&&", shell); // ← CORRIGÉ : ajout shell
		else
			ft_error_syntax("&", shell); // ← CORRIGÉ : ajout shell
	}
	else
	{
		if (current->next && current->next->str
			&& current->next->str[0] != '\0')
			ft_error_syntax(current->next->str, shell);
				// ← CORRIGÉ : ajout shell
		else
			ft_error_syntax("newline", shell); // ← CORRIGÉ : ajout shell
	}
	return (0);
}

// Traite les commandes externes (non-builtins)
static int	handle_external_command(t_token *current, t_shell *shell)
{
	current->envp = search_path(current->str, shell->env);
		// ← CORRIGÉ : shell->env
	if (!current->envp)
	{
		ft_error_cmd_not_found(current->str, shell); // ← CORRIGÉ : ajout shell
		return (0);
	}
	if (!check_command_errors(current->envp, current->str, shell))
		// ← CORRIGÉ : ajout shell
		return (0);
	return (1);
}

// Point d'entrée pour traiter une commande
int	handle_command_state(t_token *current, t_parser_state *state,
		t_shell *shell)
{
	if (!current || !state || !shell) // ← CORRIGÉ : shell au lieu de env
		return (0);
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		if (!current->str || ft_strlen(current->str) == 0)
			return (1);
		if (ft_strrchr(current->str, ' '))
		{
			ft_error_cmd_not_found(current->str, shell);
				// ← CORRIGÉ : ajout shell
			return (0);
		}
		current->role = ROLE_COMMAND;
		if (is_builtin(current))
			current->envp = NULL;
		else if (!handle_external_command(current, shell))
			// ← CORRIGÉ : shell au lieu de env
			return (0);
		*state = EXP_ARG;
		return (1);
	}
	else if (current->type == CONTENT_OPERATOR)
		return (handle_operator_error(current, shell));
			// ← CORRIGÉ : ajout shell
	return (1);
}
