
#include "minishell.h"

// * Ajoute un caractère à la fin d'une chaîne et libère l'ancienne
// * @param str: chaîne source (sera libérée)
// * @param c: caractère à ajouter
// * @return: nouvelle chaîne avec le caractère ajouté
char	*add_char_and_free(char *str, char c)
{
	char	*new;
	int		i;

	i = 0;
	new = malloc(ft_strlen(str) + 2);
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;
	new[i + 1] = '\0';
	free(str);
	return (new);
}

// * Gère l'expansion de $? dans les guillemets doubles
// * @param result: chaîne résultat actuelle
// * @param i: pointeur vers l'index (modifié)
// * @return: nouvelle chaîne avec le code de sortie ajouté
char	*handle_exit_status_in_quotes(char *result, int *i)
{
	char	*value;
	char	*new_result;

	(*i)++;                                 // Passer le '?'
	value = get_exit_status_string();       // Obtenir le code de sortie
	new_result = ft_strjoin(result, value); // Concaténer au résultat
	free(result);                           // Libérer l'ancien résultat
	free(value);                            // Libérer la valeur
	return (new_result);
}

// * Gère l'expansion d'une variable dans les guillemets doubles
// * @param result: chaîne résultat actuelle
// * @param str: chaîne source
// * @param i: pointeur vers l'index (modifié)
// * @param env: environnement pour chercher la variable
// * @return: nouvelle chaîne avec la variable expandée
char	*handle_variable_in_quotes(char *result, char *str, int *i,
		t_parse_ctx *ctx)
{
	int		start;
	char	*name;
	char	*value;
	char	*new_result;

	start = *i; // Marquer début du nom de variable
	// Lire nom variable (alphanumériques + underscore)
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start) // Si aucun nom valide trouvé
		return (result);
	name = ft_substr_len(str, start, *i - start); // Extraire le nom
	if (!name)                                    // Si échec allocation
	{
		free(result);
		return (NULL);
	}
	value = get_var_value(ctx->env, ctx->localvar, name);
		// Chercher dans l'environnement
	if (!value)                                          
		// Si variable non définie
		value = ft_strdup("");                           
			// Utiliser chaîne vide
	new_result = ft_strjoin(result, value);              
		// Concaténer au résultat
	free(result);                                        
		// Libérer ancien résultat
	free(name);                                           // Libérer le nom
	free(value);                                          // Libérer la valeur
	return (new_result);
}

// * Expanse une variable simple (VAR ou $?)
// * @param str: chaîne source
// * @param i: pointeur vers l'index (modifié)
// * @param env: environnement
// * @param result: chaîne résultat actuelle
// * @return: nouvelle chaîne avec variable expandée
char *expand_variable_simple(char *str, int *i, t_parse_ctx *ctx, char *result)
{
	(*i)++;
	if (str[*i] == '?')
		result = handle_exit_status_in_quotes(result, i);
	else
		result = handle_variable_in_quotes(result, str, i, ctx); // ← CORRIGÉ !
	return (result);
}

// * Gère l'expansion de variables dans les guillemets doubles
// * @param str: chaîne entre guillemets doubles
// * @param env: environnement pour l'expansion
// * @return: chaîne avec toutes les variables expandées
char *handle_expand_in_quotes(char *str, t_parse_ctx *ctx)
{
	char *result;
	int i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = expand_variable_simple(str, &i, ctx, result); // ← CORRIGÉ !
		else if (str[i] == '$' && str[i + 1] && ft_isdigit(str[i + 1]))
		{
			i++;
			while (str[i] && ft_isdigit(str[i]))
				i++;
		}
		else
		{
			result = add_char_and_free(result, str[i]);
			i++;
		}
	}
	return (result);
}
