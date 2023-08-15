# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/25 10:48:24 by pgouasmi          #+#    #+#              #
#    Updated: 2023/08/09 18:04:19 by pgouasmi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT_PATH	=	./includes/

LIBFT_FILE	=	libft.a

LIBFT_LIB	=	$(addprefix $(LIBFT_PATH), $(LIBFT_FILE))

HEADER = minishell.h

CC = cc

FLAG = -Wall -Wextra -Werror -ggdb3

C_FILE		=	minishell.c					\
				./utils/get_paths.c 		\
				./utils/resources.c 		\
				./utils/free.c 				\
				./utils/sig_handler.c 		\
				./utils/tokenizer.c         \
				./commands/env_cmd.c 		\

SRC			=	$(addprefix $(SRC_DIR),$(C_FILE))

OBJ			=	$(SRC:.c=.o)

all:	$(NAME)

lib:	
	@make -C $(LIBFT_PATH)

%.o:	%.c $(HEADER) 
			$(CC) $(FLAG) -I . -c $< -o $@

$(NAME): lib $(OBJ)
	$(CC) $(OBJ) $(OBJS) $(LIBFT_LIB) -lreadline -o $(NAME)

clean:
	@make clean -C $(LIBFT_PATH)
	@rm -f $(OBJ) $(OBJS)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_PATH)

re :
	make fclean
	make
