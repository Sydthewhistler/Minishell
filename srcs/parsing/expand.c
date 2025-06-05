/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:15:19 by cprot             #+#    #+#             */
/*   Updated: 2025/06/05 12:23:07 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// * Recherche une variable d'environnement par nom
// * @param env: liste chaînée des variables d'environnement
// * @param name: nom de la variable à rechercher
// * @return: valeur dupliquée de la variable, ou NULL si non trouvée
char	*get_env_value(t_env *env, char *name)
{
	t_env	*cur;

	cur = env;  // Commencer au début de la liste
	while (cur) // Parcourir toute la liste
	{
		if (ft_strcmp(cur->name, name) == 0) // Si le nom correspond
			return (ft_strdup(cur->value));  // Retourner une copie de la valeur
		cur = cur->next;                     // Passer au suivant
	}
	return (NULL); // Variable non trouvée
}

// * Obtient le code de sortie sous forme de chaîne
// * @return: chaîne représentant g_exit_status
char	*get_exit_status_string(void)
{
	extern int g_exit_status;        // Variable globale du code de sortie
	return (ft_itoa(g_exit_status)); // Convertir en chaîne
}

// * Gère l'expansion de $? (code de sortie)
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
void	handle_exit_status(int *i, t_token **tokens)
{
	char	*value;

	value = get_exit_status_string();          // Obtenir le code de sortie
	(*i)++;                                    // Passer le caractère '?'
	create_token(tokens, value, CONTENT_WORD); // Créer un token avec la valeur
	free(value);                               // Libérer la chaîne allouée
}

// * Gère l'expansion d'une variable normale ($VAR)
// * @param line: ligne de commande
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
// * @param env: environnement pour chercher la variable
void	handle_variable(char *line, int *i, t_token **tokens, t_env *env)
{
	char	*name;
	char	*value;
	int		start;

	start = *i; // Marquer le début du nom de variable
	// Lire le nom de variable (lettres, chiffres, underscore)
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (*i == start) // Si aucun caractère valide trouvé
		return ;
	name = ft_substr_len(line, start, *i - start); // Extraire le nom
	value = get_env_value(env, name);              // Chercher la valeur
	if (!value)                                    // Si variable non définie
		value = ft_strdup("");                     // Utiliser chaîne vide
	create_token(tokens, value, CONTENT_WORD); // Créer token avec la valeur
	free(name);                                // Libérer le nom
	if (value)                                 // Libérer la valeur si allouée
		free(value);
}

// * Parse une variable d'environnement ($VAR ou $?)
// * @param line: ligne de commande
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
// * @param env: environnement pour l'expansion
void	parse_var(char *line, int *i, t_token **tokens, t_env *env)
{
	(*i)++; // Passer le caractère '$'

	if (line[*i] == '?') // Si c'est $?
	{
		handle_exit_status(i, tokens); // Gérer le code de sortie
		return ;
	}

	if (ft_isdigit(line[*i])) // Si c'est $0, $1, etc. (paramètres positionnels)
		return ;               // Ignorer pour l'instant

	handle_variable(line, i, tokens, env); // Gérer variable normale
	skip_whitespace(line, i);              // Ignorer espaces suivants
}