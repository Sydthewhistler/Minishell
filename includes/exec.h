/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scavalli <scavalli@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:12 by scavalli          #+#    #+#             */
/*   Updated: 2025/05/28 15:18:40 by scavalli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
#define EXEC_H

void	error(char *str);
char	*ft_strndup(char *str, int n);
void	free_localvar(t_localvar *localvar);

void	ft_echo(char *cmd, t_token *lst);
void	ft_pwd(void);
void	ft_cd(char *line);
void	ft_env(t_env *env);

t_localvar	*is_env(t_env *env, char *name);

void	ft_localvar(t_localvar **localvar, t_token *token);
void	ft_export(t_env **env, t_token *token, t_localvar **localvar);

t_localvar	*is_local(t_localvar *local, char *name);











#endif