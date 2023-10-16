# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 17:01:08 by carolina          #+#    #+#              #
#    Updated: 2023/10/16 12:43:58 by octonaute        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#modifier le makefile de la libft maintenant que je sais en faire des mieux?
#ça ne crée pas le directory binaries
#je n'arrives pas à compiler la libft correctement donc l'ai mise en commentaires

.DEFAULT_GOAL := all
#je ne me souviens pas à quoi servait la ligne au dessus. Tester sans.

# --- COLOR ---
GREEN			= '\033[0;32m'
RESET_COLOR		= '\033[0m'

NAME = minishell

# INCLUDES = includes \

HEADER = includes/minishell.h

SRC = src/main.c \
	src/parsing/history.c \
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
	src/parsing/errors.c \
	src/parsing/utils.c \
	src/parsing/malloc.c \
	src/parsing/cmd_types.c \
	src/parsing/cd.c \
	src/parsing/pwd.c \
	src/parsing/dollar.c \
	src/parsing/echo.c \
	
SRC_OBJS = $(SRC:.c=.o)

# BINARIES = ${addprefix binaries/, ${SRC}}

# BINARIES_OBJS = ${addprefix binaries_objs/, ${SRC}}

# MAKELIB = @make -C libft
# LIBFT_A = libft.a
PATH_LIBFT = "libft"
LIBFT = $(PATH_LIBFT)/libft.a

# MAKEBIN = @mkdir -p binaries_objs/

CC = cc

RM = rm -f

CFLAGS = -gdwarf-4 #-Wall -Wextra -Werror -g3
#PENSER À ENLEVER -G3 (juste pour gdb)
MAKEFLAGS   += --no-print-directory

#OÙ DOIS-JE METTRE $(HEADER)??? POUR QUE ÇA RECOMPILE TOUT QD JE MODIFIE LE .H

binaries/%.o : src/%.c $(HEADER)
	mkdir -p binaries
	$(CC) $(CFLAGS) -c $< -o $@ 
# ${addprefix -I, ${INCLUDES}}

all: $(NAME) $(LIBFT)
#clear
	@toilet COMPILED -F border -f wideterm
#@echo $(GREEN) "\nCOMPILED\n" $(RESET_COLOR)

$(NAME): $(SRC_OBJS) $(LIBFT)
	@toilet MINISHELL -F border -f wideterm
	$(CC) $(CFLAGS) -o $(NAME) $(SRC_OBJS) $(LIBFT) -lreadline
# $(CC) $(CFLAGS) -o $(NAME) $(SRC_OBJS) -libft
# $(CC) $(CFLAGS) -o $(NAME) $(SRC_OBJS)
# $(CC) $(CFLAGS) -o $(NAME) $(SRC_OBJS) -lreadline //
# $(CC) $(CFLAGS) $(NAME) $(SRC_OBJS) $(LIBFT)

$(LIBFT):
	@toilet LIBFT -F border -f wideterm
	@make -C $(PATH_LIBFT)
	
# # binaries_objs/%.o: binaries/%.c ${HEADER} libft/libft.a | binaries_objs
# binaries_objs/%.o: binaries/%.c ${HEADER} | binaries_objs
# 	${CC} ${CFLAGS} -c $< -o $@ ${addprefix -I, ${INCLUDES}}

# binaries:
# 	${MAKEBIN}
	
clean:
	@toilet CLEAN -F border -f wideterm
	${RM} -r binaries_objs
	$(RM) $(SRC_OBJS)
	make clean -C $(PATH_LIBFT)
#clear

fclean: clean
	@toilet FCLEAN -F border -f wideterm
	$(RM) $(NAME)
	make fclean -C $(PATH_LIBFT)
#clear

re: fclean all

.PHONY: all clean fclean re

# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./minishell