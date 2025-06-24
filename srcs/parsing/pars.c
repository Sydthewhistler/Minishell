/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:07 by cprot             #+#    #+#             */
/*   Updated: 2025/06/24 11:10:16 by cprot            ###   ########.fr       */
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
	char temp[1024]; // Buffer temporaire pour stocker la chaîne
	char *str;       // Chaîne finale à retourner
	int j;           // Index pour le buffer temporaire
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
void	parse_quoted(char *s, int *i, t_token **tokens, t_parse_ctx *ctx)
{
	char	*str;
	char	*expanded;
	char	c;

	c = s[*i];
	str = build_quoted_string(s, i, c);
	if (!str)
		return ;
	if (c == '"')
	{
		expanded = handle_expand_in_quotes(str, ctx); // ← CORRIGÉ !
		free(str);
		str = expanded;
	}
	create_token(tokens, str, CONTENT_QUOTED);
	free(str);
}


void	parse_heredoc(char *s, int *i, t_token **tokens)
{
	char	*delimiter;
	char	*heredoc_content;

	*i += 2;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!s[*i] || s[*i] == '\n' || s[*i] == '|' || s[*i] == '<' || s[*i] == '>')
		return (create_token(tokens, "<<", CONTENT_OPERATOR));
	delimiter = extract_delimiter(s + *i);
	if (!delimiter || !delimiter[0])
	{
		if (delimiter)
			free(delimiter);
		return (create_token(tokens, "<<", CONTENT_OPERATOR));
	}
	heredoc_content = handle_heredoc(delimiter);
	create_token(tokens, "<<", CONTENT_OPERATOR);
	create_token(tokens, heredoc_content, CONTENT_HEREDOC);
	free(heredoc_content);
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\n')
		(*i)++;
	free(delimiter);
}

void	parse_operator(char *line, int *i, t_token **tokens)
{
	char s[3]; // Augmenté pour gérer les opérateurs de 2 caractères
	// Vérifier d'abord les opérateurs de 2 caractères
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		create_token(tokens, ">>", CONTENT_OPERATOR);
		(*i) += 2;
	}
	else if (line[*i] == '<' && line[*i + 1] == '>')
	{
		create_token(tokens, "<>", CONTENT_OPERATOR);
		(*i) += 2;
	}
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		// Laisser parse_heredoc gérer
		parse_heredoc(line, i, tokens);
		return ; // Important : ne pas appeler skip_whitespace
	}
	else
	{
		// Opérateur simple : |, <, >
		s[0] = line[*i];
		s[1] = '\0';
		create_token(tokens, s, CONTENT_OPERATOR);
		(*i)++;
	}
	skip_whitespace(line, i);
}

//  * Fonction principale de parsing d'une ligne de commande
//  * Analyse chaque caractère et délègue le parsing selon le contexte
//  * @param line: ligne de commande à parser
//  * @param tokens: liste des tokens à construire
//  * @param env: environnement pour l'expansion de variables
int	parse_line(char *line, t_token **tokens, t_env *env, t_localvar *localvar)
{
	int			i;
	t_parse_ctx	ctx;

	// Créer le contexte de parsing
	ctx.env = env;
	ctx.localvar = localvar;
	i = 0;
	skip_whitespace(line, &i);
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			parse_quoted(line, &i, tokens, &ctx); // ← CORRIGÉ !
			i++;
			skip_whitespace(line, &i);
		}
		else if (line[i] == '<' && line[i + 1] == '<')
			parse_heredoc(line, &i, tokens);
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			parse_operator(line, &i, tokens);
		else if (line[i] == '$')
			parse_var(line, &i, tokens, &ctx); // ← CORRIGÉ !
		else
			parse_word(line, &i, tokens);
	}
	if (apply_role(tokens, env) != 0)
		return (0);
	return (1);
}
