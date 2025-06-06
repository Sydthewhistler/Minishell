/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:53:50 by cprot             #+#    #+#             */
/*   Updated: 2025/06/06 19:11:00 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction utilitaire pour libérer un tableau de chaînes de caractères
void	free_args(char **args)
{
	int	i;

	if (!args)
		return ; // Protection contre NULL
	i = 0;
	// Libère chaque chaîne du tableau
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	// Libère le tableau lui-même
	free(args);
}

// Crée un chemin complet en joignant un répertoire et un nom de commande
// Exemple : create_path("/usr/bin", "ls") → "/usr/bin/ls"
char	*create_path(char *dir, char *cmd)
{
	char	*path;
	char	*temp;

	// Joint le répertoire avec "/"
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL); // Échec d'allocation
	// Joint le résultat avec le nom de la commande
	path = ft_strjoin(temp, cmd);
	free(temp); // Libère la chaîne temporaire
	return (path); // Retourne le chemin complet
}

// Cherche une commande dans tous les répertoires du PATH
// path_dirs contient quelque chose comme "/usr/bin:/bin:/usr/local/bin"
char	*find_in_path(char *cmd, char *path_dirs)
{
	int	i;

	char **dirs;     // Tableau des répertoires du PATH
	char *full_path; // Chemin complet testé
	// Divise le PATH en répertoires (séparés par ':')
	dirs = ft_split(path_dirs, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	// Teste chaque répertoire
	while (dirs[i])
	{
		// Crée le chemin complet : répertoire + "/" + commande
		full_path = create_path(dirs[i], cmd);
		if (!full_path)
			return (free_args(dirs), NULL); // Échec d'allocation
		// Teste si le fichier existe et est exécutable
		if (access(full_path, X_OK) == 0)
			return (free_args(dirs), full_path); // Trouvé ! Retourne le chemin
		// Pas trouvé dans ce répertoire, libère et continue
		free(full_path);
		i++;
	}
	// Commande non trouvée dans aucun répertoire
	free_args(dirs);
	return (NULL);
}

// Fonction principale pour résoudre le chemin d'une commande
char	*search_path(char *cmd, t_env *env)
{
	t_env	*current;

	current = env;
	// Si la commande contient '/', c'est un chemin absolu ou relatif
	// Exemple : "./script.sh", "/bin/ls", "folder/program"
	if (ft_strrchr(cmd, '/'))
		return (ft_strdup(cmd)); // Retourne une copie du chemin tel quel
	// Sinon, cherche dans la variable d'environnement PATH
	while (current)
	{
		// Trouve la variable PATH dans l'environnement
		if (ft_strcmp(current->name, "PATH") == 0)
		{
			// Cherche la commande dans tous les répertoires du PATH
			return (find_in_path(cmd, current->value));
		}
		current = current->next;
	}
	return (NULL); // PATH non trouvé dans l'environnement
}

// Gère l'état EXP_CMD (on attend une commande)
int	handle_command_state(t_token *current, t_parser_state *state, t_env *env)
{
	// Si c'est un mot ou une chaîne quotée
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		// Assigne le rôle de commande
		current->role = ROLE_COMMAND;
		// Résout le chemin de la commande (cherche dans PATH ou utilise le chemin donné)
		current->envp = search_path(current->str, env);
		// Si la commande n'est pas trouvée, affiche une erreur
		if (!current->envp)
			printf("%s: command not found\n", current->str);
		// Passe à l'état suivant (on attend maintenant des arguments)
		*state = EXP_ARG;
		return (1); // Succès
	}
	// Si on trouve un opérateur là où on attend une commande
	else if (current->type == CONTENT_OPERATOR)
	{
		// Erreur de syntaxe : on ne peut pas commencer par un opérateur
		printf("minishell: syntax error near unexpected token '%s'\n",
			current->str);
		return (0); // Échec
	}
	return (1); // Succès (autres types de tokens)
}

/*
RÉSUMÉ DU FONCTIONNEMENT :

1. RÉSOLUTION DE CHEMIN :
   - Si la commande contient '/' → chemin absolu/relatif, on l'utilise tel quel
   - Sinon → on cherche dans les répertoires du PATH

2. RECHERCHE DANS PATH :
   - Récupère la variable PATH de l'environnement
   - Divise PATH en répertoires (séparés par ':')
  
	- Pour chaque répertoire : teste si répertoire/commande existe et est exécutable
   - Retourne le premier chemin valide trouvé

3. GESTION D'ÉTAT :
   - En état EXP_CMD, on attend une commande
   - Si on trouve une commande → résout son chemin et passe en EXP_ARG
   - Si on trouve un opérateur → erreur de syntaxe

EXEMPLES :
- "ls" → cherche dans PATH → trouve "/bin/ls"
- "./script.sh" → chemin relatif → utilise tel quel
- "/usr/bin/gcc" → chemin absolu → utilise tel quel
- "invalidcmd" → cherche dans PATH → "command not found"
- "| ls" → erreur de syntaxe (commence par un opérateur)
*/