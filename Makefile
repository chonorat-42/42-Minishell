# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 13:10:41 by chonorat          #+#    #+#              #
#    Updated: 2023/08/24 18:25:25 by pgouasmi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

OS = $(shell uname | cat)

NAME = minishell
CFLAGS = -Wall -Wextra -Werror -ggdb3
RM = @rm -rf
CC = @cc
DIR = @mkdir -p
PRINT = @echo
HEADER = Includes/minishell.h
LIBFT = Libft/libft.a
MAKE_LIBFT = @make -C Libft
CLEAN_LIBFT = @make clean -C Libft
FCLEAN_LIBFT = @make fclean -C Libft
FILES = minishell\
		Commands/execution\
		Commands/built_in/echo\
		Commands/built_in/cd\
		Commands/built_in/env\
		Commands/built_in/unset\
		Commands/built_in/pwd\
		Utils/free\
		Utils/get_envp\
		Utils/get_paths\
		Utils/get_input\
		Utils/resources\
		Utils/tokenizer\
		Utils/sig_handler\
		Utils/parsing\
		Utils/expand\
		Utils/manage_quotes
SRCS = $(addsuffix .c, $(addprefix Sources/, $(FILES)))
OBJS = $(addsuffix .o, $(addprefix Objects/, $(FILES)))

$(NAME): $(OBJS)
ifeq ($(OS),Linux)
	$(PRINT) "\n${_YELLOW}Checking Libft...${_END}"
	$(MAKE_LIBFT)
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJS) -o $(NAME) $(LIBFT)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.${_END}"
else
	$(PRINT) "This $(NAME) was made for Linux only.\n"
endif

Objects/%.o: Sources/%.c Makefile $(HEADER)
ifeq ($(OS),Linux)
	$(DIR) Objects
	$(DIR) Objects/Commands Objects/Utils Objects/Commands/built_in
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(CFLAGS) $< -o $@
else
	$(PRINT) "OS=$(OS)" > .OS
endif

all: $(NAME)

clean:
	$(CLEAN_LIBFT)
	$(PRINT) "\n${_BOLD}Cleaning Objects...${_END}"
	$(RM) .OS
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
	$(RM) .OS
	$(PRINT) "${_GREEN}Objects cleaned.${_END}"
	$(PRINT) "${_GREEN}$(NAME) deleted.${_END}"
	$(PRINT) "${_GREEN}Objects directory deleted.\n${_END}"

re: fclean all

exec : all
	./minishell

.PHONY: all clean fclean re exec
