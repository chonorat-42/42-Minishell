# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 13:10:41 by chonorat          #+#    #+#              #
#    Updated: 2023/08/17 16:41:31 by chonorat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

NAME = minishell
CFLAGS = -Wall -Wextra -Werror
RM = @rm -rf
AR = @ar -rcs
CC = @cc
DIR = @mkdir -p
PRINT = @echo
HEADER = Includes/minishell.h
LIBFT = Libft/libft.a
MAKE_LIBFT = @make -C Libft
CLEAN_LIBFT = @make clean -C Libft
FCLEAN_LIBFT = @make fclean -C Libft
FILES = minishell\
		Commands/env_cmd\
		Utils/free\
		Utils/get_paths\
		Utils/resources\
		Utils/tokenizer
SRCS = $(addsuffix .c, $(addprefix Sources/, $(FILES)))
OBJS = $(addsuffix .o, $(addprefix Objects/, $(FILES)))

$(NAME): $(OBJS)
	$(PRINT) "\n${_YELLOW}Checking Libft...${_END}"
	$(MAKE_LIBFT)
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJS) -o $(NAME) $(LIBFT)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.${_END}"

Objects/%.o: Sources/%.c Makefile $(HEADER)
	$(DIR) Objects
	$(DIR) Objects/Commands Objects/Utils
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

clean:
	$(CLEAN_LIBFT)
	$(PRINT) "\n${_BOLD}Cleaning Objects...${_END}"
	$(RM) $(OBJS)
	$(PRINT) "${_BOLD}${_GREEN}Objects cleaned.${_END}"

fclean:
	$(FCLEAN_LIBFT)
	$(PRINT) "\n${_BOLD}Cleaning Objects...${_END}"
	$(RM) $(OBJS)
	$(PRINT) "${_RED}Deleting $(NAME)...${_END}"
	$(RM) $(NAME)
	$(PRINT) "${_RED}Deleting Objects directory...${_END}"
	$(RM) Objects
	$(PRINT) "${_GREEN}Objects cleaned.${_END}"
	$(PRINT) "${_GREEN}$(NAME) deleted.${_END}"
	$(PRINT) "${_GREEN}Objects directory deleted.\n${_END}"

re: fclean all

.PHONY: all clean fclean re