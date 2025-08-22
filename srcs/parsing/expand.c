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
	char		*uid_env;

	// Cas spécial pour UID
	if (ft_strcmp(name, "UID") == 0)
	{
		uid_env = getenv("UID"); // getenv() EST AUTORISÉ !
		if (uid_env)
			return (ft_strdup(uid_env));
		else
			return (ft_strdup("1000")); // Valeur par défaut
	}
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

// * Gère l'expansion de $? (code de sortie)
// * @param i: pointeur vers l'index actuel (modifié)
// * @param tokens: liste des tokens à enrichir
// * @param shell: structure shell pour accéder au exit_code
void	handle_exit_status(int *i, t_token **tokens, t_shell *shell)
{
	char	*value;

	value = ft_itoa(shell->exit_code); // Direct, plus simple !
	(*i)++;
	create_token(tokens, value, CONTENT_WORD);
	free(value);
}

void	handle_variable(char *line, int *i, t_token **tokens, t_shell *shell)
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
	value = get_var_value(shell->env, *(shell->localvar), name);
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
// * @param shell: structure shell pour l'expansion
void	parse_var(char *line, int *i, t_token **tokens, t_shell *shell)
{
	(*i)++; // Passer le $
	if (line[*i] == '?')
	{
		handle_exit_status(i, tokens, shell);
		return ;
	}
	if (ft_isdigit(line[*i]))
	{
		(*i)++;
		create_token(tokens, "", CONTENT_WORD);
		return ;
	}
	// Vérifier aussi les guillemets simples
	if (line[*i] == '"' || line[*i] == '\'')
		return ;
	if (!line[*i] || (!ft_isalpha(line[*i]) && line[*i] != '_'))
	{
		// $ isolé ou $ suivi d'un caractère invalide
		create_token(tokens, "$", CONTENT_WORD);
		return ;
	}
	handle_variable(line, i, tokens, shell);
	skip_whitespace(line, i, shell);
}
