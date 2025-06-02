/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:12 by scavalli          #+#    #+#             */
/*   Updated: 2025/06/02 13:53:00 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H
#include <stdbool.h>

/*                MASTER FUNCTIONS              */

int	exec_master(t_token *token, t_env **env, t_localvar **localvar);
int	ft_builtin(t_token *token, t_env **env, t_localvar **localvar, int pipe[2]);

/*             GENERAL UTILS FUNCTIONS                */

void	error(char *str);
char	*ft_strndup(char *str, int n);
bool	which_built_in(t_token *token, t_env **env, t_localvar **localvar);

/*                BUILT_IN FUNCTIONS                */

void	ft_echo(char *cmd, t_token *lst);
void	ft_pwd(void);
void	ft_cd(char *line);
void	ft_env(t_env *env);
void	unset(t_localvar **localvar, t_env **env, t_token *token);
void	ft_export(t_env **env, t_token *token, t_localvar **localvar);
void	ft_localvar(t_localvar **localvar, t_token *token); // variables local

/*               BUILT_IN UTILS FUNCTIONS            */

t_localvar	*is_local(t_localvar *local, char *name);
t_localvar	*is_env(t_env *env, char *name);
void	free_localvar(t_localvar *localvar);










#endif