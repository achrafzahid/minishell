# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 03:28:20 by amabbadi          #+#    #+#              #
#    Updated: 2025/05/02 13:08:16 by amabbadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-DDEBUG -fsanitize=address
RM = rm -f

PARSE_DIR = ./parsing
EXEC_DIR = ./execution
LIBFT_UTILS = ./libft_utils
GLOB_UTILS_DIR = ./global_utils
DEBUG = ./debug
SIGNALS = ./signals
BUILTINS = ./builtins


######################################
SRCS = $(PARSE_DIR)/arrayallocator.c \
       $(PARSE_DIR)/commandeparser.c \
       $(PARSE_DIR)/hollyparser.c    \
       $(PARSE_DIR)/loader.c         \
       $(PARSE_DIR)/main.c           \
       $(PARSE_DIR)/p_com_split.c    \
       $(PARSE_DIR)/parserlexer.c    \
       $(PARSE_DIR)/pipe_split.c     \
       $(PARSE_DIR)/syntax.c         \
       $(PARSE_DIR)/wildcards.c      \
       $(EXEC_DIR)/execute_all.c     \
       $(EXEC_DIR)/find_access.c      \
       $(EXEC_DIR)/redirections.c    \
       $(EXEC_DIR)/shlvl.c           \
       $(LIBFT_UTILS)/ft_itoa.c      \
       $(LIBFT_UTILS)/ft_memset.c    \
       $(LIBFT_UTILS)/ft_split.c     \
       $(LIBFT_UTILS)/ft_strcat.c    \
       $(LIBFT_UTILS)/ft_strchr.c    \
       $(LIBFT_UTILS)/ft_strcmp.c    \
       $(LIBFT_UTILS)/ft_strcpy.c    \
       $(LIBFT_UTILS)/ft_strdup.c    \
       $(LIBFT_UTILS)/ft_strjoin.c   \
       $(LIBFT_UTILS)/ft_strncmp.c   \
       $(LIBFT_UTILS)/ft_strnstr.c   \
       $(LIBFT_UTILS)/ft_substr.c    \
       $(LIBFT_UTILS)/ft_strlen.c    \
       $(LIBFT_UTILS)/ft_atoi_safe.c    \
       $(GLOB_UTILS_DIR)/utils_builtin_handle.c \
       $(GLOB_UTILS_DIR)/utils_comandparser.c   \
       $(GLOB_UTILS_DIR)/utils_env.c            \
       $(GLOB_UTILS_DIR)/utils_execute_all.c    \
       $(GLOB_UTILS_DIR)/utils_general.c        \
       $(GLOB_UTILS_DIR)/utils_hollyparser.c    \
       $(GLOB_UTILS_DIR)/utils_hollyparser_quotes.c  \
       $(GLOB_UTILS_DIR)/utils_leak.c                \
       $(GLOB_UTILS_DIR)/utils_main.c                \
       $(GLOB_UTILS_DIR)/utils_p_com_split.c         \
       $(GLOB_UTILS_DIR)/utils_process.c             \
       $(DEBUG)/debug.c                              \
       $(SIGNALS)/signals.c                          \
       $(BUILTINS)/cd.c                              \
       $(BUILTINS)/echo.c                            \
       $(BUILTINS)/env.c                             \
       $(BUILTINS)/exiter.c                          \
       $(BUILTINS)/export.c                          \
       $(BUILTINS)/pwd.c                             \
       $(BUILTINS)/unset.c                           \
######################################################

OBJS = $(SRCS:.c=.o)

all: $(NAME)
	$(RM) $(OBJS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all 

.PHONY: clean
