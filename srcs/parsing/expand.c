/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coraline <coraline@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:15:19 by cprot             #+#    #+#             */
/*   Updated: 2025/08/03 18:47:43 by coraline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ancienne fonction - QUE l'environnement (pour les builtins)
char	*get_env_value(t_env *env, char *name)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (ft_strcmp(cur->name, name) == 0)
			return (ft_strdup(cur->value));
		cur = cur->next;
	}
	return (NULL);
}

// Nouvelle fonction - Variables locales + environnement (pour l'expansion)
char	*get_var_value(t_env *env, t_localvar *localvar, char *name)
{
	t_localvar	*local;

	// Chercher d'abord dans les variables locales
	local = localvar;
	while (local)
	{
		if (ft_strcmp(local->name, name) == 0)
			return (ft_strdup(local->value));
		local = local->next;
	}
	// Si pas trouvé, chercher dans l'environnement
	return (get_env_value(env, name));
}

// * Obtient le code de sortie sous forme de chaîne
// * @return: chaîne représentant gstatus
char	*get_exit_status_string(void)
{
	return (ft_itoa(g_signal));
}

// * Gère l'expansion de $? (code de sortie)
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
void	handle_exit_status(int *i, t_token **tokens)
{
	char	*value;

	value = get_exit_status_string();
	(*i)++;
	create_token(tokens, value, CONTENT_WORD);
	free(value);
}

void	handle_variable(char *line, int *i, t_token **tokens, t_parse_ctx *ctx)
{
	char	*name;
	char	*value;
	int		start;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (*i == start)
		return ;
	name = ft_substr_len(line, start, *i - start);
	value = get_var_value(ctx->env, ctx->localvar, name); // ← CORRIGÉ !
	if (!value)
		value = ft_strdup("");
	create_token(tokens, value, CONTENT_WORD);
	free(name);
	if (value)
		free(value);
}

// * Parse une variable d'environnement ($VAR ou $?)
// * @param line: ligne de commande
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
// * @param env: environnement pour l'expansion
// * Parse une variable d'environnement ($VAR ou $?)
void	parse_var(char *line, int *i, t_token **tokens, t_parse_ctx *ctx)
{
	(*i)++; // Passer le $

	if (line[*i] == '?')
	{
		handle_exit_status(i, tokens);
		return ;
	}
	if (ft_isdigit(line[*i]))
		return ;
	if (!line[*i] || (!ft_isalpha(line[*i]) && line[*i] != '_'))
	{
		// $ isolé ou $ suivi d'un caractère invalide
		create_token(tokens, "$", CONTENT_WORD);
		return ;
	}

	handle_variable(line, i, tokens, ctx);
	skip_whitespace(line, i);
}
