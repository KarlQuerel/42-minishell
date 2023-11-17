# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 17:01:08 by carolina          #+#    #+#              #
#    Updated: 2023/11/17 18:03:32 by casomarr         ###   ########.fr        #
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
# HFLAGS  preprocessor flags
# DIR_DUP   duplicate directory tree

SRC_DIR     := src
OBJ_DIR     := obj
INCLUDE_DIR := includes

SRCS		:=  src/main.c \
				src/builtins/builtins_errors.c \
				src/builtins/cd.c \
				src/builtins/cd2.c \
				src/builtins/dollar.c \
				src/builtins/echo.c \
				src/builtins/env.c \
				src/builtins/exit.c \
				src/builtins/export.c \
				src/builtins/history.c \
				src/builtins/pwd.c \
				src/builtins/unset.c \
				src/executable/exec.c \
				src/executable/exec_utils.c \
				src/executable/exec_errors.c \
				src/executable/redirect.c \
				src/executable/exec_continued.c \
				src/parsing/commands.c \
				src/parsing/parsing.c \
				src/parsing/parsing2.c \
				src/parsing/erase_spaces.c \
				src/parsing/checks.c \
				src/parsing/signal.c \
				src/parsing/env_list.c \
				src/parsing/lstnew.c \
				src/parsing/free.c \
				src/parsing/errors.c \
				src/parsing/errors2.c \
				src/parsing/utils.c \
				src/parsing/utils2.c \
				src/parsing/malloc.c \
				src/parsing/cmd_types.c \
				src/parsing/prompt.c \
				#get_next_line/get_next_line_bonus.c \
				#get_next_line/get_next_line_utils_bonus.c \
				

OBJS 		:= $(addprefix $(OBJ_DIR),  $(addsuffix .o, $(SRC_FILES)))
OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
		
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g3
HFLAGS    	:= -I$(INCLUDE_DIR)
RFLAGS     	:= -lreadline
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
	$(CC) $(CFLAGS) $(HFLAGS) $^ -o $@ $(RFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(OBJ_DIR)/parsing $(OBJ_DIR)/executable $(OBJ_DIR)/builtins
	$(CC) $(CFLAGS) $(HFLAGS) -c $< -o $@

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

v : minishell
	@valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --trace-children=yes ./minishell

sv : minishell
	@valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell
