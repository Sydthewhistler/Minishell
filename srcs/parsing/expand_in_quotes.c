
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
	new = malloc(ft_strlen(str) + 2); // +1 pour le char, +1 pour '\0'
	if (!new)                         // Si échec allocation
	{
		free(str); // Libérer l'ancienne chaîne
		return (NULL);
	}
	while (str[i]) // Copier la chaîne existante
	{
		new[i] = str[i];
		i++;
	}
	new[i] = c;        // Ajouter le nouveau caractère
	new[i + 1] = '\0'; // Terminer la chaîne
	free(str);         // Libérer l'ancienne chaîne
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
char	*handle_variable_in_quotes(char *result, char *str, int *i, t_env *env)
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
	value = get_env_value(env, name); // Chercher dans l'environnement
	if (!value)                       // Si variable non définie
		value = ft_strdup("");        // Utiliser chaîne vide
	new_result = ft_strjoin(result, value); // Concaténer au résultat
	free(result);                           // Libérer ancien résultat
	free(name);                             // Libérer le nom
	free(value);                            // Libérer la valeur
	return (new_result);
}

// * Expanse une variable simple (VAR ou $?)
// * @param str: chaîne source
// * @param i: pointeur vers l'index (modifié)
// * @param env: environnement
// * @param result: chaîne résultat actuelle
// * @return: nouvelle chaîne avec variable expandée
char	*expand_variable_simple(char *str, int *i, t_env *env, char *result)
{
	(*i)++; // Passer le '$'
	if (str[*i] == '?') // Si c'est $?
		result = handle_exit_status_in_quotes(result, i);
	else // Sinon variable normale
		result = handle_variable_in_quotes(result, str, i, env);
	return (result);
}

// * Gère l'expansion de variables dans les guillemets doubles
// * @param str: chaîne entre guillemets doubles
// * @param env: environnement pour l'expansion
// * @return: chaîne avec toutes les variables expandées
char	*handle_expand_in_quotes(char *str, t_env *env)
{
	char *result;
	int i;

	result = ft_strdup(""); // Commencer avec chaîne vide
	i = 0;

	while (str[i]) // Parcourir toute la chaîne
	{
		// Si $ suivi de ?, lettre ou underscore = variable à expander
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = expand_variable_simple(str, &i, env, result);

		// Si $ suivi de chiffres = paramètres positionnels (ignorés)
		else if (str[i] == '$' && str[i + 1] && ft_isdigit(str[i + 1]))
		{
			i++;                                 // Passer le '$'
			while (str[i] && ft_isdigit(str[i])) // Ignorer tous les chiffres
				i++;
		}

		// Caractère normal : l'ajouter tel quel
		else
		{
			result = add_char_and_free(result, str[i]); // Ajouter le caractère
			i++;
		}
	}
	return (result);
}