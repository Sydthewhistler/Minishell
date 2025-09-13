/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprot <cprot@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:17:44 by cprot             #+#    #+#             */
/*   Updated: 2025/09/13 09:17:45 by cprot            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// CONTENT types (ce que contient le token)
# define CONTENT_WORD 1
# define CONTENT_QUOTED 2
# define CONTENT_OPERATOR 3
# define CONTENT_HEREDOC 4

// ROLE types (fonction du token dans la commande)
# define ROLE_COMMAND 1
# define ROLE_ARGUMENT 2
# define ROLE_FILENAME 3
# define ROLE_PIPE 4
# define ROLE_REDIRECT_IN 5
# define ROLE_REDIRECT_OUT 6
# define ROLE_REDIRECT_APPEND 7
# define ROLE_REDIRECT_HEREDOC 8
# define ROLE_HEREDOC_CONTENT 9

// CODES D'ERREUR (comme bash)
# define SUCCESS 0
# define SYNTAX_ERROR_CODE 2
# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define SIGNAL_INTERRUPTED 130

// Variable globale pour les signaux
extern volatile sig_atomic_t	g_signal;

// ENUM pour l'état du parseur
typedef enum e_parser_state
{
	EXP_CMD,
	EXP_ARG,
	EXP_FILE
}								t_parser_state;

// Structure pour les tokens
typedef struct s_token
{
	char						*str;
	int							type;
	int							role;
	char						*envp;
	int							exit_code;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

// Structure pour les variables d'environnement
typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
	struct s_env				*prev;
}								t_env;

// Structure pour les variables locales
typedef struct s_localvar
{
	char						*name;
	char						*value;
	struct s_localvar			*next;
	struct s_localvar			*prev;
}								t_localvar;

// NOUVELLE STRUCTURE CENTRALE (remplace g_signal pour les codes d'erreur)
typedef struct s_shell
{
	t_env						*env;
	t_localvar					**localvar;
	int							exit_code;
	int							should_exit;
}								t_shell;

// UTILS
void							create_token(t_token **tokens, char *content,
									int content_type);
void							skip_whitespace(char *line, int *i,
									t_shell *shell);
int								parse_line(char *line, t_token **tokens,
									t_shell *shell);
void							create_and_advance(t_token **tokens, char *op,
									int *i, int len);
char							*extract_delimiter(char *s);
char							*add_line_to_result(char *result, char *line);
void							process_heredoc_content(char *delimiter,
									t_token **tokens, char *s, int *i);
t_token							*found_last(t_token *tokens);

// QUOTES
char							*extract_quoted_content(char *line, int *i,
									t_shell *shell);
char							*append_quoted_content(char *result, char *line,
									int *i, t_shell *shell);

// PARSING HELPERS
char							*append_char(char *str, char c);
char							*append_variable_content(char *result,
									char *line, int *i, t_shell *shell);
char							*parse_complete_segment(char *line, int *i,
									t_shell *shell);

// HEREDOC
char							*extract_delimiter(char *s);
char							*handle_heredoc(char *delimiter);

// ENV
t_env							*init_env_from_envp(char **envp);
char							*get_env_value(t_env *env, char *name);
char							*get_var_value(t_env *env, t_localvar *localvar,
									char *name);

// EXPAND
void							handle_exit_status(int *i, t_token **tokens,
									t_shell *shell);
void							handle_variable(char *line, int *i,
									t_token **tokens, t_shell *shell);
void							parse_var(char *line, int *i, t_token **tokens,
									t_shell *shell);
char							*handle_expand_in_quotes(char *str,
									t_shell *shell);

// ROLE
int								apply_role(t_token **tokens, t_shell *shell);
int								handle_command_state(t_token *current,
									t_parser_state *state, t_shell *shell);
bool							is_builtin(t_token *token);

// SEARCH PATH
char							*search_path(char *cmd, t_env *env);
void							free_args(char **args);
char							*create_path(char *dir, char *cmd);
char							*find_in_path(char *cmd, char *path_dirs);

// SIGNAL
void							ft_setup_interactive_signal(void);
void							ft_setup_execution_signal(void);
void							ft_setup_heredoc_signals(void);

// ERROR
void							ft_error_cmd_not_found(char *cmd,
									t_shell *shell);
void							ft_error_permission_denied(char *cmd,
									t_shell *shell);
void							ft_error_file(char *cmd, int is_dir,
									t_shell *shell);
void							ft_error_syntax(char *token, t_shell *shell);
void							putstr_error(char *str);
void							check_exit_code(t_token *tokens,
									t_shell *shell);
bool							check_error(t_token *tokens);

// FREE
void							free_token(t_token **tokens);
void							free_all_env(t_env **env);
void							free_all_localvar(t_localvar **localvar);

// VALIDATE BUILT IN
int								validate_builtin_arguments(t_token *tokens,
									t_shell *shell);
int								is_numeric(char *str);
int								is_valid_identifier(char *name);
int								validate_unset(t_token *token, t_shell *shell,
									char *cmd);
int								validate_export(t_token *token, t_shell *shell,
									char *cmd);
int								validate_cd_args(t_token *token,
									t_shell *shell);
int								validate_exit_args(t_token *token,
									t_shell *shell);

#endif