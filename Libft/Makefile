# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/14 14:37:20 by chonorat          #+#    #+#              #
#    Updated: 2023/08/20 18:59:47 by pgouasmi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

NAME = libft.a
CFLAGS = -Wall -Wextra -Werror
RM = @rm -rf
AR = @ar -rcs
CC = @cc -c
DIR = @mkdir -p
PRINT = @echo
HEADER = ../Includes/libft.h
FILES = ft_isalpha\
		ft_isdigit\
		ft_isalnum\
		ft_isascii\
		ft_isprint\
		ft_isws\
		ft_strlen\
		ft_memset\
		ft_bzero\
		ft_memcpy\
		ft_memmove\
		ft_strlcpy\
		ft_strlcat\
		ft_toupper\
		ft_tolower\
		ft_strchr\
		ft_strrchr\
		ft_strncmp\
		ft_strcmp\
		ft_memchr\
		ft_memcmp\
		ft_strnstr\
		ft_atoi\
		ft_calloc\
		ft_strdup\
		ft_substr\
		ft_strjoin\
		ft_strtrim\
		ft_split\
		ft_itoa\
		ft_strmapi\
		ft_striteri\
		ft_putchar_fd\
		ft_putstr_fd\
		ft_putendl_fd\
		ft_putnbr_fd\
		ft_arr_size\
		ft_arr_copy\
		ft_char_index\
		ft_lstnew\
		ft_lstadd_front\
		ft_lstsize\
		ft_lstlast\
		ft_lstadd_back\
		ft_lstdelone\
		ft_lstclear\
		ft_lstiter\
		ft_lstmap\
		ft_strjoin_free_first\
		ft_strjoin_free_both\
		ft_free_null\
		ft_printf/ft_printf\
		ft_printf/ft_putchar_pf\
		ft_printf/ft_putnbr_base_p\
		ft_printf/ft_putnbr_base\
		ft_printf/ft_putstr_pf\
		ft_dprintf/ft_dprintf\
		ft_dprintf/ft_dputnbr_base\
		ft_dprintf/ft_putchar_dpf\
		ft_dprintf/ft_putnbr_base_dp\
		ft_dprintf/ft_putstr_dpf\
		get_next_line/get_next_line\
		get_next_line/get_next_line_utils
SRCS = $(addsuffix .c, $(FILES))
OBJ_FILES = $(addsuffix .o, $(addprefix ../Objects/Libft/, $(FILES)))

$(NAME): $(OBJ_FILES)
	$(PRINT) "\nMaking ${_BOLD}libft.a${_END}..."
	$(AR) $(NAME) $(OBJ_FILES)
	$(PRINT) "${_BOLD}${_GREEN}libft.a done.\a${_END}"

../Objects/Libft/%.o: %.c Makefile $(HEADER)
	$(DIR) ../Objects
	$(DIR) ../Objects/Libft
	$(DIR) ../Objects/Libft/ft_printf
	$(DIR) ../Objects/Libft/ft_dprintf
	$(DIR) ../Objects/Libft/get_next_line
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) $(CFLAGS) $< -o $@

all: $(NAME)

clean:
	$(PRINT) "${_BOLD}Cleaning libft...${_END}"
	$(RM) $(OBJ_FILES)
	$(PRINT) "${_GREEN}libft cleaned.${_END}"

fclean:
	$(PRINT) "${_RED}Cleaning libft...${_END}"
	$(PRINT) "${_RED}Deleting libft objects directory...${_END}"
	$(PRINT) "${_RED}Deleting $(NAME)...${_END}"
	$(RM) $(OBJ_FILES)
	$(RM) ../Objects/Libft
	$(RM) $(NAME)
	$(PRINT) "${_GREEN}Libft cleaned.${_END}"
	$(PRINT) "${_GREEN}Libft Objects directory deleted.${_END}"
	$(PRINT) "${_GREEN}libft.a deleted.${_END}"

re: fclean all

.PHONY: all clean fclean re
