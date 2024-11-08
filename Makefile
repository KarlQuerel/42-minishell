# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 17:01:08 by carolina          #+#    #+#              #
#    Updated: 2023/12/08 01:17:37 by kquerel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------#
#   COLORS                                       #
#------------------------------------------------#
GREEN			= '\033[0;32m'
RESET_COLOR		= '\033[0m'

NAME		= minishell

SRC_DIR		= src
OBJ_DIR		= obj
INCLUDE_DIR	= includes

SRCS		=	src/main.c \
				src/init_and_utils.c \
				src/builtins/builtins_errors.c \
				src/builtins/cd.c \
				src/builtins/dollar.c \
				src/builtins/dollar2.c \
				src/builtins/dollar3.c \
				src/builtins/echo.c \
				src/builtins/env.c \
				src/builtins/exit.c \
				src/builtins/exit_2.c \
				src/builtins/export.c \
				src/builtins/export_2.c \
				src/builtins/history.c \
				src/builtins/pwd.c \
				src/builtins/unset.c \
				src/executable/exec.c \
				src/executable/exec_2.c \
				src/executable/exec_3.c \
				src/executable/exec_utils.c \
				src/executable/exec_utils_2.c \
				src/executable/exec_errors.c \
				src/executable/heredoc.c \
				src/executable/heredoc_utils.c \
				src/executable/pipe_and_dup.c \
				src/executable/redirect.c \
				src/executable/redirect_utils.c \
				src/parsing/commands.c \
				src/parsing/parsing.c \
				src/parsing/parsing2.c \
				src/parsing/parsing3.c \
				src/parsing/erase_spaces.c \
				src/parsing/checks.c \
				src/parsing/signal.c \
				src/parsing/env_list.c \
				src/parsing/lstnew.c \
				src/parsing/free.c \
				src/parsing/free_2.c \
				src/parsing/errors.c \
				src/parsing/errors2.c \
				src/parsing/utils.c \
				src/parsing/utils2.c \
				src/parsing/malloc.c \
				src/parsing/cmd_types.c \
				src/parsing/prompt.c \
				src/parsing/determine_cmd.c \
				src/parsing/determine_cmd_2.c \
				

OBJS 		= $(addprefix $(OBJ_DIR),  $(addsuffix .o, $(SRC_FILES)))
OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
HFLAGS		= -I$(INCLUDE_DIR)
RFLAGS		= -lreadline
MAKEFLAGS	+= --no-print-directory

LIBFT		= libft/libft.a

ifeq ($(debug), true)
	CFLAGS	+= -fsanitize=address,undefined
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@toilet COMPILED -F border -f wideterm
	@toilet MINISHELL -F border -f wideterm
	$(CC) $(CFLAGS) $(HFLAGS) $^ -o $@ $(RFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/executable $(OBJ_DIR)/builtins
	$(CC) $(CFLAGS) $(HFLAGS) -c $< -o $@

$(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/executable $(OBJ_DIR)/builtins:
	mkdir -p $@

$(LIBFT):
	make -C libft

clean:
	$(RM) -r $(OBJ_DIR)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all