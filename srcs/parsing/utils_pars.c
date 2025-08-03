/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 10:12:38 by cprot             #+#    #+#             */
/*   Updated: 2025/08/03 18:57:49 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// * Ignore les espaces, tabulations et caractères spéciaux (:, !)
// * @param line: ligne de commande
// * @param i: pointeur vers l'index actuel (modifié)
void	skip_whitespace(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t' || line[*i] == ':'
		|| line[*i] == '\\')
		(*i)++;
	// Cas spécial pour !
	if (line[*i] == '!')
	{
		g_signal = 1;             // ! donne exit code 1
		(*i)++;                   // Avancer pour l'ignorer
		skip_whitespace(line, i); // Continuer à ignorer les espaces après
	}
}

// * Trouve le dernier token de la liste chaînée
// * @param tokens: premier token de la liste
// * @return: pointeur vers le dernier token, ou NULL si liste vide
t_token	*found_last(t_token *tokens)
{
	if (!tokens) // Si la liste est vide
		return (NULL);
	while (tokens->next) // Parcourir jusqu'au dernier élément
		tokens = tokens->next;
	return (tokens); // Retourner le dernier token
}

// * Wrapper pour créer un token d'opérateur
// * @param tokens: liste des tokens à enrichir
// * @param operator: chaîne de l'opérateur
// * @param content_type: type de contenu du token
void	create_operator_token(t_token **tokens, char *operator,
		int content_type)
{
	create_token(tokens, operator, content_type); // Déléguer à create_token
}

// * Crée et ajoute un nouveau token à la fin de la liste
// * @param tokens: pointeur vers la liste des tokens
// * @param content: contenu du token à créer
// * @param content_type: type du token (WORD, OPERATOR, etc.)
void	create_token(t_token **tokens, char *content, int content_type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token)); // Allouer un nouveau token
	if (!new_token || !tokens)
		// Vérifier l'allocation et les paramètres
		return ;
	new_token->str = ft_strdup(content); // Copier le contenu
	if (!new_token->str)                 // Vérifier l'allocation de la chaîne
	{
		free(new_token);
		return ;
	}
	new_token->type = content_type; // Définir le type
	new_token->role = 0;            // Initialiser le rôle à 0
	new_token->envp = NULL;
	new_token->next = NULL; // Pas de suivant pour l'instant
	new_token->prev = NULL; // Pas de précédent pour l'instant
	if (!(*tokens))         // Si c'est le premier token de la liste
		*tokens = new_token;
	else // Sinon, l'ajouter à la fin
	{
		last = found_last(*tokens); // Trouver le dernier token
		last->next = new_token;     // Lier le dernier au nouveau
		new_token->prev = last;
		// Lier le nouveau au dernier (liste doublement chaînée)
	}
}

// * Parse un mot (séquence de caractères sans espaces ni opérateurs)
// * @param line: ligne de commande
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
void	parse_word(char *line, int *i, t_token **tokens)
{
	char	*str;
	int		start;

	start = *i; // Marquer le début du mot
	// Avancer tant qu'on ne rencontre pas un caractère spécial
	while (line[*i] != '\0' && line[*i] != ' ' && line[*i] != '\t'
		&& line[*i] != '>' && line[*i] != '<' && line[*i] != '$'
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	if (*i > start) // Si on a trouvé au moins un caractère
	{
		str = ft_substr_len(line, start, *i - start); // Extraire la sous-chaîne
		create_token(tokens, str, CONTENT_WORD);
		// Créer un token de type WORD
		free(str);
	}
	skip_whitespace(line, i); // Ignorer les espaces suivants
}
