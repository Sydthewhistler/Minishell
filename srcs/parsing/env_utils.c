#include "minishell.h"

// * Trouve le dernier nœud de la liste d'environnement
// * @param tokens: premier nœud de la liste
// * @return: pointeur vers le dernier nœud, ou NULL si liste vide
t_env	*found_envplast(t_env *tokens)
{
	if (!tokens) // Si liste vide
		return (NULL);
	while (tokens->next) // Parcourir jusqu'au dernier
		tokens = tokens->next;
	return (tokens);// Retourner le dernier nœud
}

// * Libère un nœud d'environnement en cas d'erreur d'allocation
// * @param new: nœud à libérer partiellement
static void	free_node_on_error(t_env *new)
{
	if (new->name) // Si le nom a été alloué
		free(new->name);
	if (new->value) // Si la valeur a été allouée
		free(new->value);
	free(new);// Libérer le nœud lui-même
}

// * Crée et ajoute un nouveau nœud d'environnement à la liste
// * @param envp: pointeur vers la liste d'environnement
// * @param name: nom de la variable
// * @param value: valeur de la variable
// * @return: nouveau nœud créé, ou NULL en cas d'erreur
t_env	*create_node(t_env **envp, char *name, char *value)
{
	t_env	*new;
	t_env	*last;

	new = (t_env *)malloc(sizeof(t_env));// Allouer le nouveau nœud
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);// Dupliquer le nom
	new->value = ft_strdup(value);// Dupliquer la valeur
	if (!new->name || !new->value) // Si échec d'allocation
	{
		free_node_on_error(new);// Nettoyer partiellement
		return (NULL);
	}
	new->prev = NULL;// Pas de précédent pour l'instant
	new->next = NULL;// Pas de suivant pour l'instant
	if (!(*envp)) // Si c'est le premier nœud
		*envp = new;
	else// Sinon l'ajouter à la fin
	{
		last = found_envplast(*envp);// Trouver le dernier
		last->next = new;// Lier le dernier au nouveau
		new->prev = last;// Lier le nouveau au dernier
	}
	return (new);
}

// * Sépare une chaîne "NAME=VALUE" en nom et valeur
// * @param envp: chaîne format "NAME=VALUE"
// * @param name: pointeur vers le nom (alloué)
// * @param value: pointeur vers la valeur (allouée)
void	split_envp(char *envp, char **name, char **value)
{
	int	i;
	int	start;

	i = 0;
	while (envp[i] && envp[i] != '=') // Chercher le '='
		i++;
	if (!envp[i]) // Si pas de '=' trouvé
	{
		*name = NULL;
		*value = NULL;
		return ;
	}
	start = i;// Position du '='
	*name = malloc((i + 1) * sizeof(char));// Allouer pour le nom
	if (!(*name))
		return ;
	i = 0;
	while (i < start) // Copier le nom
	{
		(*name)[i] = envp[i];
		i++;
	}
	(*name)[i] = '\0';// Terminer le nom
	*value = ft_substr(envp, start + 1);// Extraire la valeur après '='
	return ;
}

// * Initialise la liste d'environnement depuis le tableau envp
// * @param envp: tableau de chaînes "NAME=VALUE" terminé par NULL
// * @return: liste chaînée d'environnement initialisée
t_env	*init_env_from_envp(char **envp)
{
	char	*name;
	char	*value;
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;// Commencer avec liste vide
	while (envp[i]) // Parcourir tout le tableau
	{
		split_envp(envp[i], &name, &value);// Séparer nom et valeur
		create_node(&env, name, value);// Créer et ajouter le nœud
		free(name);// Libérer le nom temporaire
		free(value);// Libérer la valeur temporaire
		i++;
	}
	return (env);// Retourner la liste construite
}
