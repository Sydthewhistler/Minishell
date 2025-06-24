/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:22:44 by cprot             #+#    #+#             */
/*   Updated: 2025/06/24 10:37:15 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Libère un tableau de chaînes
void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i]) // Libère chaque chaîne
	{
		free(args[i]);
		i++;
	}
	free(args); // Libère le tableau lui-même
}

// Construit un chemin complet : dir + "/" + cmd
char	*create_path(char *dir, char *cmd)
{
	char	*path;
	char	*temp;

	temp = ft_strjoin(dir, "/"); // dir + "/"
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, cmd); // dir + "/" + cmd
	free(temp);                   // Libère le temporaire
	return (path);
}

// Cherche une commande dans les répertoires du PATH
char	*find_in_path(char *cmd, char *path_dirs)
{
	int			i;
	struct stat	info;
	char		**dirs;
	char		*full_path;

	dirs = ft_split(path_dirs, ':'); // Découpe PATH en répertoires
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i]) // Teste chaque répertoire
	{
		full_path = create_path(dirs[i], cmd); // Construit le chemin
		if (!full_path)
			return (free_args(dirs), NULL);
		if (stat(full_path, &info) == 0) // Le fichier existe ?
		{
			if (S_ISREG(info.st_mode) && access(full_path, X_OK) == 0)
				// Fichier exécutable ?
				return (free_args(dirs), full_path);                  
					// Trouvé !
		}
		free(full_path); // Pas bon, on continue
		i++;
	}
	free_args(dirs);
	return (NULL); // Pas trouvé
}

// Interface principale : trouve le chemin d'une commande
char	*search_path(char *cmd, t_env *env)
{
	t_env *current;

	current = env;
	if (ft_strrchr(cmd, '/'))    // Contient déjà un chemin ?
		return (ft_strdup(cmd)); // Retourne tel quel
	while (current)              // Cherche la variable PATH
	{
		if (ft_strcmp(current->name, "PATH") == 0)
			return (find_in_path(cmd, current->value)); // Cherche dans PATH
		current = current->next;
	}
	return (NULL); // PATH pas trouvé
}
