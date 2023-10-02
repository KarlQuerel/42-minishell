SRC = 	main.c

SRCS = 	${addprefix srcs/, ${SRC}}

# OBJ = 	${addprefix binaries/, ${SRC}}

OBJS = 	${OBJ:%.c=%.o}

NAME = 	minishell

MAKELIB = @make -C libft

# MAKEBIN = @mkdir -p binaries/

MAKEFLAGS += --no-print-directory 

CC 	= 	cc

RM 	= 	rm -f

CFLAGS = -Wall -Wextra -Werror

INCLUDES = libft/ includes/

HEADER = includes/minishell.h

# binaries/%.o: srcs/%.c ${HEADER} libft/libft.a | binaries
# 	${CC} ${CFLAGS} -c $< -o $@ ${addprefix -I, ${INCLUDES}}

${NAME}: ${OBJS}
	${CC} -o ${NAME} ${OBJS} -libft

all: ${NAME}

libft/libft.a:
	${MAKELIB}

# binaries:
# 	${MAKEBIN}

bonus: all

clean:
	#  ${RM} -r binaries
	 make clean -C libft

fclean: clean
	 ${RM} ${NAME}
	 make fclean -C libft

re: fclean all

.PHONY:	all bonus clean fclean re