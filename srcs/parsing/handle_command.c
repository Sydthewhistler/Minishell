/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:53:50 by cprot             #+#    #+#             */
/*   Updated: 2025/06/24 10:37:18 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Vérifie si un chemin est valide et exécutable
static int	check_command_errors(char *envp, char *str)
{
	struct stat	info;

	if (stat(envp, &info) != 0) // Le fichier n'existe pas
		return (printf("minishell: %s: No such file or directory\n", str), 0);
	if (S_ISDIR(info.st_mode)) // C'est un répertoire
		return (printf("minishell: %s: Is a directory\n", str), 0);
	if (access(envp, X_OK) != 0) // Pas de permission d'exécution
		return (printf("minishell: %s: Permission denied\n", str), 0);
	return (1); // Tout est OK
}

// Gère les erreurs de syntaxe pour les opérateurs
static int	handle_operator_error(t_token *current)
{
	if (current->next && current->next->str && current->next->str[0] != '\0')
		printf("minishell: syntax error near unexpected token '%s'\n",
			current->next->str);
	else
		printf("minishell: syntax error near unexpected token 'newline'\n");
	return (0); // Erreur
}

// Traite les commandes externes (non-builtins)
static int	handle_external_command(t_token *current, t_env *env)
{
	current->envp = search_path(current->str, env); // Trouve le chemin complet
	if (!current->envp)                             // Commande pas trouvée
		return (printf("%s: command not found\n", current->str), 0);
	if (!check_command_errors(current->envp, current->str)) // Vérifie validité
		return (0);
	return (1); // Succès
}

// Point d'entrée pour traiter une commande
int	handle_command_state(t_token *current, t_parser_state *state, t_env *env)
{
	if (!current || !state || !env)
		return (0);
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		current->role = ROLE_COMMAND;
		if (is_builtin(current)) // Commande builtin
			current->envp = NULL;
		else if (!handle_external_command(current, env)) // Commande externe
			return (0);
		*state = EXP_ARG; // Prochaine étape : arguments
		return (1);
	}
	else if (current->type == CONTENT_OPERATOR)
		return (handle_operator_error(current));
	return (1);
}
