# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amabbadi <amabbadi@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 03:28:20 by amabbadi          #+#    #+#              #
#    Updated: 2025/04/25 15:25:41 by amabbadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-DDEBUG -fsanitize=address  
RM = rm -f

PARSE_DIR = ./parsing
EXEC_DIR = ./execution
UTILS_DIR = ./utils

SRCS = 						$(PARSE_DIR)/arrayallocator.c \
       $(PARSE_DIR)/commandeparser.c \
       $(PARSE_DIR)/main.c \
       $(PARSE_DIR)/parserlexer.c\
       $(PARSE_DIR)/debug.c\
       $(PARSE_DIR)/loader.c\
	$(PARSE_DIR)/p_com_split.c\
       $(PARSE_DIR)/leak_utils.c\
	$(PARSE_DIR)/pipe_split.c\
	$(PARSE_DIR)/hollyparser.c\
	$(PARSE_DIR)/wildcards.c\
       $(PARSE_DIR)/syntax.c\
       						$(EXEC_DIR)/env.c\
       $(EXEC_DIR)/env_utils.c\
       $(EXEC_DIR)/pwd.c\
       $(EXEC_DIR)/cd.c\
       $(EXEC_DIR)/unset.c\
       $(EXEC_DIR)/find_access.c \
       $(EXEC_DIR)/execute_all.c \
       $(EXEC_DIR)/echo.c \
       $(EXEC_DIR)/shlvl.c \
       $(EXEC_DIR)/export.c \
				$(EXEC_DIR)/redirections.c\
				$(EXEC_DIR)/exiter.c\
       						$(UTILS_DIR)/ft_strcmp.c\
       $(UTILS_DIR)/ft_strchr.c\
       $(UTILS_DIR)/ft_memset.c\
       $(UTILS_DIR)/ft_split.c \
       $(UTILS_DIR)/ft_strdup.c \
       $(UTILS_DIR)/ft_strjoin.c \
       $(UTILS_DIR)/ft_strnstr.c \
       $(UTILS_DIR)/ft_strncmp.c \


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
