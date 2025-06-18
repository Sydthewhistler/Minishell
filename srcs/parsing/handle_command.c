/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:53:50 by cprot             #+#    #+#             */
/*   Updated: 2025/06/18 11:56:23 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction utilitaire pour libérer un tableau de chaînes de caractères
void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// Crée un chemin complet en joignant un répertoire et un nom de commande
// Exemple : create_path("/usr/bin", "ls") → "/usr/bin/ls"
char	*create_path(char *dir, char *cmd)
{
	char	*path;
	char	*temp;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd);
	free(temp);
	return (path);
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
	while (dirs[i]) // Teste chaque répertoire
	{
		// Crée le chemin complet : répertoire + "/" + commande
		full_path = create_path(dirs[i], cmd);
		if (!full_path)
			return (free_args(dirs), NULL);
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
	// Input validation
	if (!current || !state || !env)
		return (0);
	// Si c'est un mot ou une chaîne quotée
	if (current->type == CONTENT_WORD || current->type == CONTENT_QUOTED)
	{
		// Assigne le rôle de commande
		current->role = ROLE_COMMAND;
		// Check if it's a builtin first (more efficient)
		if (is_builtin(current))
			current->envp = NULL; // Builtins don't need path resolution
		else
		{
			// Résout le chemin de la commande pour les commandes externes
			current->envp = search_path(current->str, env);
			if (!current->envp)
			{
				// More specific error message
				if (ft_strrchr(current->str, '/'))
					printf("%s: No such file or directory\n", current->str);
				else
					printf("%s: command not found\n", current->str);
				return (0);
			}
		}
		// Passe à l'état suivant
		*state = EXP_ARG;
		return (1);
	}
	// Si on trouve un opérateur là où on attend une commande
	else if (current->type == CONTENT_OPERATOR)
	{
		if (current->next && current->next->str
			&& current->next->str[0] != '\0')
			printf("minishell: syntax error near unexpected token '%s'\n",
				current->next->str);
		else
			printf("minishell: syntax error near unexpected token 'newline'\n");
		return (0);
	}
	return (1); // Succès pour autres types de tokens
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