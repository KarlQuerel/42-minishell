# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 17:01:08 by carolina          #+#    #+#              #
#    Updated: 2023/10/24 13:36:12 by kquerel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------#
#   COLORS                                       #
#------------------------------------------------#
GREEN			= '\033[0;32m'
RESET_COLOR		= '\033[0m'


NAME        := minishell

#------------------------------------------------#
#   DEFINITIONS                                  #
#------------------------------------------------#
# SRC_DIR   source directory
# OBJ_DIR   object directory
# SRCS      source files
# OBJS      object files
#
# CC        compiler
# CFLAGS    compiler flags
# CPPFLAGS  preprocessor flags
# DIR_DUP   duplicate directory tree

SRC_DIR     := src
OBJ_DIR     := obj
INCLUDE_DIR := includes

# SRCS        := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/parsing/*.c $(SRC_DIR)/executable/*.c $(SRC_DIR)/builtins/*.c)
# OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

SRCS		:=  src/main.c \
				src/parsing/commands.c \
				src/parsing/parsing.c \
				src/parsing/checks.c \
				src/parsing/signal.c \
				src/parsing/env_list.c \
				src/parsing/lstnew.c \
				src/parsing/free.c \
				src/executable/exec.c \
				src/executable/exec_utils.c \
				src/executable/exec_errors.c \
				src/executable/redirect.c \
				src/executable/pipes.c \
				src/builtins/cd.c \
				src/builtins/dollar.c \
				src/builtins/echo.c \
				src/builtins/env.c \
				src/builtins/export.c \
				src/builtins/history.c \
				src/builtins/pwd.c \
				src/builtins/unset.c \
				src/parsing/errors.c \
				src/parsing/utils.c \
				src/parsing/malloc.c \
				src/parsing/cmd_types.c \
				src/parsing/prompt.c

SRCS 		:= $(addprefix $(SRC_DIR)/, main.c parsing/commands.c parsing/parsing.c parsing/checks.c parsing/signal.c parsing/env_list.c parsing/lstnew.c parsing/free.c executable/exec.c executable/exec_utils.c executable/exec_errors.c executable/redirect.c executable/pipes.c builtins/cd.c builtins/dollar.c builtins/echo.c builtins/env.c builtins/export.c builtins/history.c builtins/pwd.c builtins/unset.c parsing/errors.c parsing/utils.c parsing/malloc.c parsing/cmd_types.c parsing/prompt.c)
OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))



# OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
# SRCS := $(addprefix $(SRC_DIR)/, $(notdir $(SRCS)))
# SRCS		:= $(SRCS:%=$(SRC_DIR)/*.c)
# OBJS		:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# SRCS		:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/parsing/*.c $(SRC_DIR)/executable/*.c $(SRC_DIR)/builtins/*.c)



CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g3
CPPFLAGS    := -I$(INCLUDE_DIR)
LDFLAGS     := -Llibft -lreadline
LDLIBS      := -lft
MAKEFLAGS	+= --no-print-directory

LIBFT       := libft/libft.a

.PHONY: all clean fclean re

#------------------------------------------------#
#   MAKE                                         #
#------------------------------------------------#
# all       default goal
# $(NAME)   linking .o -> binary
# %.o       compilation .c -> .o


all: $(NAME)
	@toilet COMPILED -F border -f wideterm

$(NAME): $(OBJS) $(LIBFT)
	@toilet MINISHELL -F border -f wideterm
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/executable $(OBJ_DIR)/builtins
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/executable $(OBJ_DIR)/builtins:
	mkdir -p $@

$(LIBFT):
	make -C libft

#------------------------------------------------#
#   CLEAN, RE AND PHONY                          #
#------------------------------------------------#
# RM        force remove
# clean     remove .o
# fclean    remove .o + binary
# re        remake default goal

clean:
	$(RM) -r $(OBJ_DIR)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

runv : minishell
	@valgrind --suppressions=.ignore --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes ./minishell