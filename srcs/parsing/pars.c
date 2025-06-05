/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/06/05 12:19:30 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  * Construit une chaîne de caractères entre guillemets (simples ou doubles)
//  * @param s: la chaîne source à parser
//  * @param i: pointeur vers l'index actuel dans la chaîne (modifié)
//  * @param c: le caractère de fermeture de guillemet (' ou ")
//  * @return: la chaîne extraite entre guillemets, ou NULL en cas d'erreur

static char	*build_quoted_string(char *s, int *i, char c)
{
	char	temp[1024]; // Buffer temporaire pour stocker la chaîne
	char	*str;       // Chaîne finale à retourner
	int		j;           // Index pour le buffer temporaire
	j = 0;
	(*i)++; // Passer le guillemet d'ouverture
	// Parcourir jusqu'au guillemet de fermeture
	while (s[*i] != c)
	{
		// Vérifier si on atteint la fin sans fermer les guillemets
		if (s[*i] == '\0')
		{
			write(2, "Error : unclosed quotes\n", 25);
			return (NULL);
		}
		temp[j] = s[*i]; // Copier le caractère dans le buffer
		j++;
		(*i)++;
	}
	temp[j] = '\0';      // Terminer la chaîne
	str = malloc(j + 1); // Allouer la mémoire pour la chaîne finale
	if (!str)
		return (NULL);
	ft_strlcpy(str, temp, j + 1); // Copier depuis le buffer temporaire
	return (str);
}

//  * Parse une section entre guillemets et crée un token
//  * Gère l'expansion de variables pour les guillemets doubles
//  * @param s: chaîne source
//  * @param i: pointeur vers l'index actuel
//  * @param tokens: liste des tokens à enrichir
//  * @param env: environnement pour l'expansion de variables

void	parse_quoted(char *s, int *i, t_token **tokens, t_env *env)
{
	char	*str;
	char	*expanded;
	char	c;

	c = s[*i]; // Sauvegarder le type de guillemet (' ou ")
	// Extraire le contenu entre guillemets
	str = build_quoted_string(s, i, c);
	if (!str)
		return ;
	// Si c'est des guillemets doubles, on peut faire l'expansion de variables
	if (c == '"')
	{
		expanded = handle_expand_in_quotes(str, env);
		free(str);
		str = expanded;
	}
	// Créer un token avec le contenu quoté
	create_token(tokens, str, CONTENT_QUOTED);
	free(str);
}

//  * Parse un heredoc (<<) et crée les tokens correspondants
//  * @param s: chaîne source
//  * @param i: pointeur vers l'index actuel
//  * @param tokens: liste des tokens à enrichir

void	parse_heredoc(char *s, int *i, t_token **tokens)
{
	char	*delimiter;
	char	*heredoc_content;

	*i += 2; // Passer les deux caractères "<<"
	// Extraire le délimiteur (mot après <<)
	delimiter = extract_delimiter(s + *i);
	if (!delimiter || !delimiter[0])
	{
		if (delimiter)
			free(delimiter);
		return ;
	}
	// Lire le contenu du heredoc jusqu'au délimiteur
	heredoc_content = handle_heredoc(delimiter);
	// Créer deux tokens : l'opérateur << et le contenu
	create_token(tokens, "<<", CONTENT_OPERATOR);
	create_token(tokens, heredoc_content, CONTENT_HEREDOC);
	free(heredoc_content);
	// Avancer l'index pour passer le délimiteur
	while (s[*i] == ' ' || s[*i] == '\t') // Ignorer les espaces
		(*i)++;
	// Passer le délimiteur complet
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n'
		&& s[*i] != '|' && s[*i] != '<' && s[*i] != '>')
		(*i)++;
	free(delimiter);
	skip_whitespace(s, i); // Ignorer les espaces suivants
}

//  * Parse les opérateurs de redirection et pipes
//  * @param line: ligne de commande
//  * @param i: pointeur vers l'index actuel
//  * @param tokens: liste des tokens à enrichir

void	parse_operator(char *line, int *i, t_token **tokens)
{
	char	s[2];

	s[0] = line[*i];
	s[1] = '\0';
	// Vérifier si c'est l'opérateur de redirection en ajout ">>"
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		create_token(tokens, ">>", CONTENT_OPERATOR);
		(*i) += 2; // Avancer de 2 caractères
	}
	else
	{
		// Opérateur simple : |, <, >
		create_token(tokens, s, CONTENT_OPERATOR);
		(*i)++; // Avancer d'1 caractère
	}
	skip_whitespace(line, i); // Ignorer les espaces suivants
}

//  * Fonction principale de parsing d'une ligne de commande
//  * Analyse chaque caractère et délègue le parsing selon le contexte
//  * @param line: ligne de commande à parser
//  * @param tokens: liste des tokens à construire
//  * @param env: environnement pour l'expansion de variables

void	parse_line(char *line, t_token **tokens, t_env *env)
{
	int	i;

	i = 0;
	skip_whitespace(line, &i); // Ignorer les espaces en début de ligne
	// Parcourir toute la ligne caractère par caractère
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
			// PRIORITÉ 1: Guillemets (traitement prioritaire)
		{
			parse_quoted(line, &i, tokens, env);
			i++; // Passer le guillemet de fermeture
			skip_whitespace(line, &i);
		}
		else if (line[i] == '<' && line[i + 1] == '<')
			// PRIORITÉ 2: Heredoc (doit être testé avant < simple)
			parse_heredoc(line, &i, tokens);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			// PRIORITÉ 3: Opérateurs de redirection et pipes
			parse_operator(line, &i, tokens);
		else if (line[i] == '$')
			// PRIORITÉ 4: Variables d'environnement (commençant par $)
			parse_var(line, &i, tokens, env);
		else // PRIORITÉ 5: Mots normaux (arguments, commandes, etc.)
			parse_word(line, &i, tokens);
	}
}
