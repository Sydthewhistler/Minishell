NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
LDFLAGS = -lreadline
RM = rm -f

# Directories
SRCS_DIR = srcs/
LIBFT_DIR = libft/
INCLUDES_DIR = includes/
EXEC_DIR = exec/
BUILTIN_DIR = $(EXEC_DIR)built_in/
PARSING_DIR = parsing/
UTILS_DIR = utils/

# Source files
MAIN_FILES = main.c

EXEC_FILES = exec_master.c \
	exec.c \
	manage_redirect.c \
	manage_redirect2.c

BUILTIN_FILES = add_localvar.c \
	builtin_master.c \
	ft_cd.c \
	ft_echo.c \
	ft_env.c \
	ft_export.c \
	ft_pwd.c \
	ft_unset.c \
	update_pwd.c

PARSING_FILES = env_utils.c \
	expand.c \
	expand_in_quotes.c \
	handle_command.c \
	heredoc.c \
	pars.c \
	role.c \
	utils_pars.c \
	search_path.c \
	utils_pars2.c \
	quotes.c \
	validate_builtins.c \
	validate_builtins2.c 

UTILS_FILES = env_utils.c \
	error.c \
	free.c \
	ft_utils.c \
	ft_utils2.c \
	signal.c 

# Full paths to source files
SRCS = $(addprefix $(SRCS_DIR), $(MAIN_FILES)) \
	$(addprefix $(SRCS_DIR)$(EXEC_DIR), $(EXEC_FILES)) \
	$(addprefix $(SRCS_DIR)$(BUILTIN_DIR), $(BUILTIN_FILES)) \
	$(addprefix $(SRCS_DIR)$(PARSING_DIR), $(PARSING_FILES)) \
	$(addprefix $(SRCS_DIR)$(UTILS_DIR), $(UTILS_FILES))

# Object files
OBJS = $(SRCS:.c=.o)

# Include directories
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

# Library
LIBFT = $(LIBFT_DIR)libft.a

# Headers
HEADERS = $(INCLUDES_DIR)minishell.h $(INCLUDES_DIR)exec.h

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@echo "$(NAME) compiled successfully!"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) -s

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean -s
	@$(RM) $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean -s
	@$(RM) $(NAME)

re: fclean all

# Debug rule to show which files will be compiled
show:
	@echo "Source files:"
	@echo $(SRCS)
	@echo
	@echo "Object files:"
	@echo $(OBJS)

.PHONY: all clean fclean re show