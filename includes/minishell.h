/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:26:43 by cprot             #+#    #+#             */
/*   Updated: 2025/05/26 17:07:17 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "exec.h"

# define CONTENT_WORD 1     // mot normal
# define CONTENT_QUOTED 2   // "string" ou 'string'
# define CONTENT_OPERATOR 3 // |, >, <, >>
# define CONTENT_ESCAPED 4  // \caractère

# define ROLE_COMMAND 1
# define ROLE_ARGUMENT 2
# define ROLE_FILENAME 3
# define ROLE_PIPE 4
# define ROLE_REDIRECT_IN 5
# define ROLE_REDIRECT_OUT 6

typedef struct s_token
{
	char			*s;
	int				content_type;
	int				role;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

t_list				*pars_line(char *line);

static void			create_node(t_list **stack, char *token_str,
						int token_type);

#endif