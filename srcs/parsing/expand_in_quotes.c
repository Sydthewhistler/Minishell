
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
// * @param shell: structure shell pour accéder au exit_code
// * @return: nouvelle chaîne avec le code de sortie ajouté
char	*handle_exit_status_in_quotes(char *result, int *i, t_shell *shell)
{
	char	*value;
	char	*new_result;

	(*i)++;                            // Passer le '?'
	value = ft_itoa(shell->exit_code); // Direct, plus simple !
	new_result = ft_strjoin(result, value);
	free(result);
	free(value);
	return (new_result);
}

// * Gère l'expansion d'une variable dans les guillemets doubles
// * @param result: chaîne résultat actuelle
// * @param str: chaîne source
// * @param i: pointeur vers l'index (modifié)
// * @param shell: structure shell pour l'expansion
// * @return: nouvelle chaîne avec la variable expandée
char	*handle_variable_in_quotes(char *result, char *str, int *i,
		t_shell *shell)
{
	int		start;
	char	*name;
	char	*value;
	char	*new_result;

	start = *i; // Marquer début du nom de variable
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
	value = get_var_value(shell->env, *(shell->localvar), name);
	if (!value)
		value = ft_strdup("");
	new_result = ft_strjoin(result, value);
	free(result);
	free(name);  // Libérer le nom
	free(value); // Libérer la valeur
	return (new_result);
}

// * Expanse une variable simple (VAR ou $?)
// * @param str: chaîne source
// * @param i: pointeur vers l'index (modifié)
// * @param shell: structure shell pour l'expansion
// * @param result: chaîne résultat actuelle
// * @return: nouvelle chaîne avec variable expandée
char	*expand_variable_simple(char *str, int *i, t_shell *shell, char *result)
{
	(*i)++;
	if (str[*i] == '?')
		result = handle_exit_status_in_quotes(result, i, shell);
	else
		result = handle_variable_in_quotes(result, str, i, shell);
	return (result);
}

// * Gère l'expansion de variables dans les guillemets doubles
// * @param str: chaîne entre guillemets doubles
// * @param shell: structure shell pour l'expansion
// * @return: chaîne avec toutes les variables expandées
char	*handle_expand_in_quotes(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (str[i + 1] == '?'
				|| ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = expand_variable_simple(str, &i, shell, result);
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
