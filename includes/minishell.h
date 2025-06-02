/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:43 by cprot             #+#    #+#             */
/*   Updated: 2025/06/02 10:22:46 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//# include "exec.h"
# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// CONTENT types (ce que contient le token)
# define CONTENT_WORD 1
# define CONTENT_QUOTED 2
# define CONTENT_OPERATOR 3
# define CONTENT_HEREDOC 4 // ← Ajout pour le contenu des heredocs

// ROLE types (fonction du token dans la commande)
# define ROLE_COMMAND 1
# define ROLE_ARGUMENT 2
# define ROLE_FILENAME 3
# define ROLE_PIPE 4
# define ROLE_REDIRECT_IN 5     //
# define ROLE_REDIRECT_OUT 6    // >
# define ROLE_REDIRECT_APPEND 7 // ← Ajout pour >>
# define ROLE_REDIRECT_HEREDOC 8

typedef struct s_token
{
	char			*str;
	int				type;
	int				role;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

void				create_token(t_token **tokens, char *content,
						int content_type);
void				skip_whitespace(char *line, int *i);
void				parse_line(char *line, t_token **tokens);
char				*extract_delimiter(char *s);
char				*handle_heredoc(char *delimiter);
void				update_history_entry(char *line, char *content,
						char *delimiter);

#endif
