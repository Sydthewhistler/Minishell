NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
RM = rm -f

SRCS_DIR = srcs/
LIBFT_DIR = libft/
INCLUDES_DIR = includes/
EXEC_DIR = exec/
BUILTIN_DIR = $(EXEC_DIR)built_in/
PARSING_DIR = parsing/
UTILS_DIR = utils/

SRC_FILES = main.c
EXEC_FILES = exec_master.c
BUILTIN_FILES = ft_cd.c ft_echo.c ft_env.c ft_export.c ft_pwd.c ft_unset.c
PARSING_FILES = token.c pars.c utils.c heredoc.c env_utils.c

SRCS = $(addprefix $(SRCS_DIR), $(SRC_FILES)) \
       $(addprefix $(SRCS_DIR)$(EXEC_DIR), $(EXEC_FILES)) \
       $(addprefix $(SRCS_DIR)$(BUILTIN_DIR), $(BUILTIN_FILES)) \
       $(addprefix $(SRCS_DIR)$(PARSING_DIR), $(PARSING_FILES)) \
       $(addprefix $(SRCS_DIR)$(UTILS_DIR), $(UTILS_FILES))

OBJS = $(SRCS:.c=.o)

INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)libft.a
HEADERS = $(INCLUDES_DIR)minishell.h $(INCLUDES_DIR)exec.h

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@echo "$(NAME) compiled successfully!"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

parsing: $(LIBFT)
	cc -Wall -Wextra -Werror -Iincludes -Ilibft srcs/main.c srcs/parsing/*.c -o minishell -Llibft -lft -lreadline

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re parsing