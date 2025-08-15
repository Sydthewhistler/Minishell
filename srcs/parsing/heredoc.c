/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:56:44 by cprot             #+#    #+#             */
/*   Updated: 2025/08/15 17:39:51 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// * Ajoute une nouvelle ligne au résultat existant du heredoc
// * Gère l'ajout du caractère de nouvelle ligne entre les lignes
// * @param result: chaîne résultat actuelle
// * @param line: nouvelle ligne à ajouter
// * @return: nouvelle chaîne contenant result + "\n" + line
char	*add_line_to_result(char *result, char *line)
{
	char	*new_result;
	char	*temp;

	// CORRIGÉ: Vérification de result avant d'accéder à result[0]
	if (result && result[0] != '\0')
	{
		temp = ft_strjoin(result, "\n");
		free(result);
		if (!temp) // CORRIGÉ: Vérification du malloc
			return (NULL);
		result = temp;
	}
	new_result = ft_strjoin(result, line);
	if (result) // CORRIGÉ: Vérification avant free
		free(result);
	return (new_result);
}

// * Gère la lecture interactive d'un heredoc
// * Lit les lignes jusqu'à rencontrer le délimiteur
// * @param delimiter: mot-clé qui termine la saisie du heredoc
// * @return: chaîne contenant tout le contenu saisi
char	*handle_heredoc(char *delimiter)
{
	char	*result;
	char	*temp;
	char	*final_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (1)
	{
		temp = readline("heredoc> "); // Afficher le prompt et lire une ligne
		if (!temp)                    // Si EOF (Ctrl+D), sortir proprement
		{
			write(1, "\n", 1); // Afficher un retour à la ligne
			break ;
		}
		if (ft_strcmp(temp, delimiter) == 0)
		// Si la ligne correspond au délimiteur, arrêter
		{
			free(temp);
			break ;
		}
		result = add_line_to_result(result, temp);
		// Ajouter la ligne au résultat
		free(temp);
	}
	if (result && result[0] != '\0')
	{
		final_result = ft_strjoin(result, "\n");
		free(result);
		return (final_result);
	}
	return (result);
}

// * Extrait le délimiteur après l'opérateur <<
// * Ignore les espaces/tabs en début et s'arrête aux caractères séparateurs
// * @param s: chaîne source contenant le délimiteur
// * @return: délimiteur extrait (alloué dynamiquement)
char	*extract_delimiter(char *s)
{
	int		i;
	char	*delimiter;

	int start; // Position de début du délimiteur
	int len;   // Longueur du délimiteur
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		// Ignorer les espaces et tabulations en début
		i++;
	start = i;
	// Marquer le début du délimiteur
	while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
		// Lire jusqu'à un caractère séparateur
		i++;
	len = i - start; // Calculer la longueur
	delimiter = malloc(sizeof(char) * (len + 1));
	// Allouer la mémoire pour le délimiteur
	if (!delimiter)
		return (NULL); // Copier le délimiteur caractère par caractère
	i = 0;
	while (i < len)
	{
		delimiter[i] = s[start + i];
		i++;
	}
	delimiter[len] = '\0'; // Terminer la chaîne
	return (delimiter);
}

// * Construit l'entrée d'historique complète pour un heredoc
// * Format: ligne_originale\ncontenu_heredoc\ndélimiteur
// * @param line: ligne de commande originale
// * @param content: contenu du heredoc saisi
// * @param delimiter: délimiteur utilisé
// * @return: chaîne formatée pour l'historique
char	*build_heredoc_history(char *line, char *content, char *delimiter)
{
	char	*history;
	char	*temp;

	temp = ft_strjoin(line, "\n");       // Étape 1: line + "\n"
	history = ft_strjoin(temp, content); // Étape 2: (line + "\n") + content
	free(temp);
	temp = history; // Étape 3: (line + "\n" + content) + "\n"
	history = ft_strjoin(temp, "\n");
	free(temp);
	temp = history; // Étape 4: (line + "\n" + content + "\n") + delimiter
	history = ft_strjoin(temp, delimiter);
	free(temp);
	return (history);
}

// * Met à jour l'entrée d'historique pour inclure le contenu du heredoc
// * Remplace la dernière entrée d'historique par une version complète
// * @param line: ligne de commande originale
// * @param content: contenu du heredoc
// * @param delimiter: délimiteur du heredoc
void	update_history_entry(char *line, char *content, char *delimiter)
{
	char		*history_entry;
	HIST_ENTRY	*last_entry;

	history_entry = build_heredoc_history(line, content, delimiter);
	// Construire la nouvelle entrée d'historique complète
	last_entry = remove_history(history_length - 1);
	// Supprimer la dernière entrée de l'historique
	if (last_entry)
	{ // Libérer la mémoire de l'ancienne entrée
		free(last_entry->line);
		free(last_entry);
	}
	add_history(history_entry); // Ajouter la nouvelle entrée complète
	free(history_entry);
}
