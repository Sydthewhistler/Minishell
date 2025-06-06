/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   role.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:41:09 by cprot             #+#    #+#             */
/*   Updated: 2025/06/06 19:09:12 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction qui assigne les rôles spécifiques aux opérateurs de redirection et pipe
void	assign_operator_roles(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	// Parcourt tous les tokens de la liste
	while (current)
	{
		// Si le token est identifié comme un opérateur
		if (current->type == CONTENT_OPERATOR)
		{
			// Assigne le rôle spécifique selon l'opérateur trouvé
			if (ft_strcmp(current->str, "|") == 0)
				current->role = ROLE_PIPE; // Pipe pour connecter des commandes
			else if (ft_strcmp(current->str, "<<") == 0)
				current->role = ROLE_REDIRECT_HEREDOC;
					// Here-document (lecture multi-ligne)
			else if (ft_strcmp(current->str, ">>") == 0)
				current->role = ROLE_REDIRECT_APPEND;
					// Redirection en mode ajout
			else if (ft_strcmp(current->str, "<") == 0)
				current->role = ROLE_REDIRECT_IN; // Redirection d'entrée
			else if (ft_strcmp(current->str, ">") == 0)
				current->role = ROLE_REDIRECT_OUT; // Redirection de sortie
		}
		current = current->next;
	}
}

// Gère l'état EXP_ARG (on attend un argument ou un opérateur)
static int	handle_argument_state(t_token *current, t_parser_state *state)
{
	// Si c'est un mot ou une chaîne quotée
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		current->role = ROLE_ARGUMENT; // C'est un argument de commande
	}
	// Si c'est un opérateur
	else if (current->type == CONTENT_OPERATOR)
	{
		if (current->role == ROLE_PIPE)
			*state = EXP_CMD; // Après un pipe, on attend une nouvelle commande
		else if (current->role == ROLE_REDIRECT_OUT
			|| current->role == ROLE_REDIRECT_APPEND
			|| current->role == ROLE_REDIRECT_IN)
			*state = EXP_FILE; // Après une redirection, on attend un nom de fichier
		else if (current->role == ROLE_REDIRECT_HEREDOC)
			*state = EXP_ARG; // Après <<, on attend le délimiteur (traité comme argument)
	}
	return (1); // Succès
}

// Gère l'état EXP_FILE (on attend un nom de fichier après une redirection)
static int	handle_filename_state(t_token *current, t_parser_state *state)
{
	// Si c'est un mot ou une chaîne quotée
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		current->role = ROLE_FILENAME; // C'est un nom de fichier
		*state = EXP_ARG;// Retour à l'état normal après avoir trouvé le fichier
	}
	// Si c'est un opérateur au lieu d'un nom de fichier
	else if (current->type == CONTENT_OPERATOR)
	{
		// Erreur de syntaxe : on attendait un fichier, pas un opérateur
		printf("minishell: syntax error near unexpected token '%s'\n",
			current->str);
		return (0); // Échec - erreur de syntaxe
	}
	return (1); // Succès
}

// Fonction principale qui applique les rôles à tous les tokens selon le contexte
void	apply_role(t_token **tokens, t_env *env)
{
	t_parser_state	state;
	t_token			*current;

	current = *tokens;
	state = EXP_CMD; // On commence en attendant une commande
	// D'abord, assigne les rôles aux opérateurs
	assign_operator_roles(tokens);
	// Ensuite, parcourt tous les tokens pour assigner les autres rôles
	while (current)
	{
		if (state == EXP_CMD)
		{
			// On attend une commande
			if (!handle_command_state(current, &state, env))
				return ; // Erreur lors du traitement de la commande
		}
		else if (state == EXP_ARG)
		{
			// On attend un argument
			if (!handle_argument_state(current, &state))
				return ; // Erreur lors du traitement de l'argument
		}
		else if (state == EXP_FILE)
		{
			// On attend un nom de fichier
			if (!handle_filename_state(current, &state))
				return ; // Erreur lors du traitement du fichier
		}
		current = current->next;
	}
}

/*
RÉSUMÉ DU FONCTIONNEMENT :

1. assign_operator_roles() : Identifie et assigne les rôles aux opérateurs (|,
	<, >, <<, >>)

2. La fonction apply_role() utilise une machine à états avec 3 états :
   - EXP_CMD : attend une commande
   - EXP_ARG : attend un argument
   - EXP_FILE : attend un nom de fichier

3. Les transitions d'états se font selon les règles :
   - Après une commande → EXP_ARG (arguments)
   - Après un pipe → EXP_CMD (nouvelle commande)
   - Après une redirection → EXP_FILE (nom de fichier)
   - Après un fichier → EXP_ARG (retour normal)

4. Gestion d'erreurs : Si on trouve un opérateur quand on attend un fichier,
   c'est une erreur de syntaxe.

Exemple : "ls -l > file.txt | grep test"
- "ls" → ROLE_COMMAND (EXP_CMD → EXP_ARG)
- "-l" → ROLE_ARGUMENT
- ">" → ROLE_REDIRECT_OUT (EXP_ARG → EXP_FILE)
- "file.txt" → ROLE_FILENAME (EXP_FILE → EXP_ARG)
- "|" → ROLE_PIPE (EXP_ARG → EXP_CMD)
- "grep" → ROLE_COMMAND (EXP_CMD → EXP_ARG)
- "test" → ROLE_ARGUMENT
*/