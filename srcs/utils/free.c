#include "minishell.h"

void	free_token(t_token **tokens)
{
	t_token	*cur;
	t_token	*tmp;

	if (!tokens)
		return ;
	cur = *tokens;
	while (cur)
	{
		tmp = cur->next;
		if (cur->str)
			free(cur->str);
		if (cur->envp)
			free(cur->envp);
		free(cur);
		cur = tmp;
	}
	*tokens = NULL;
}

void	free_all_env(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	if (!env)
		return ;
	cur = *env;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*env = NULL;
}

void	free_all_localvar(t_localvar **localvar)
{
	t_localvar	*cur;
	t_localvar	*tmp;

	if (!localvar)
		return ;
	cur = *localvar;
	while (cur)
	{
		tmp = cur->next;
		if (cur->name)
			free(cur->name);
		if (cur->value)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*localvar = NULL;
}

void	create_and_advance(t_token **tokens, char *op, int *i, int len)
{
	create_token(tokens, op, CONTENT_OPERATOR);
	*i += len;
}

void	process_heredoc_content(char *delimiter, t_token **tokens, char *s,
		int *i)
{
	char *heredoc_content;
	int delimiter_len;

	// Obtenir le contenu du heredoc (pour traitement interactif)
	heredoc_content = handle_heredoc(delimiter);
	// Créer l'opérateur heredoc
	create_token(tokens, "<<", CONTENT_OPERATOR);
	// Créer un token pour le contenu du heredoc
	if (heredoc_content && heredoc_content[0] != '\0')
		create_token(tokens, heredoc_content, CONTENT_HEREDOC);
	else
		create_token(tokens, "", CONTENT_HEREDOC);
			// Token vide si pas e contenu
	// Le token aura le bon type et ne sera pas exécuté comme commande
	// Avancer l'index correctement
	delimiter_len = ft_strlen(delimiter);
	*i += delimiter_len;
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	// Libérer la mémoire du contenu
	if (heredoc_content)
		free(heredoc_content);
}
