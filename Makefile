# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 13:10:41 by chonorat          #+#    #+#              #
#    Updated: 2023/10/25 15:58:45 by pgouasmi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLOR
_GREEN = \033[92m
_YELLOW = \033[33m
_RED = \033[31m

#POLICE
_END = \033[0m
_BOLD = \033[1m

OS = $(shell uname)

NAME = minishell
CFLAGS = -Wall -Wextra -Werror -IIncludes -ggdb3
RM = @rm -rf
CC = @cc
DIR = @mkdir -p
PRINT = @echo
HEADER = Includes/minishell.h
LIBFT = Libft/libft.a
MAKE_LIBFT = @make -C Libft
CLEAN_LIBFT = @make clean -C Libft
FCLEAN_LIBFT = @make fclean -C Libft
NORM = @norminette | awk '$$NF!="OK!" {print "$(RED)" $$0 "\033[0;0m"}'
FILES = minishell\
		Environment/get_envp\
		Environment/get_paths\
		Environment/create_envp\
		Environment/get_svar\
		Environment/update_shlvl\
		Environment/update_envp\
		Parsing/parsing\
		Parsing/Quotes/manage_quotes\
		Parsing/Expand/expand\
		Parsing/Expand/expand_dlst\
		Tokenizer/tokenizer\
		Tokenizer/heredoc\
		Tokenizer/parse_tokens\
		Tokenizer/prints\
		Tokenizer/Redirections/tokens_dlist\
		Tokenizer/Redirections/tokens_fd\
		Tokenizer/Redirections/remove_redirections\
		Tokenizer/Cmd_arrays/cmd_arr\
		Get_input/get_input\
		Execution/execution\
		Execution/binary_exec\
		Execution/pipe\
		Execution/is_builtin\
		Execution/Built_in/echo\
		Execution/Built_in/cd\
		Execution/Built_in/env\
		Execution/Built_in/unset\
		Execution/Built_in/Export/export\
		Execution/Built_in/Export/export_utils\
		Execution/Built_in/Export/add_to_env\
		Execution/Built_in/Export/join_content\
		Execution/Built_in/pwd\
		Execution/Built_in/Exit/exit\
		Execution/Built_in/Exit/exit_utils\
		Signals/default_sig\
		Signals/heredoc_sig\
		Signals/ignore_sig\
		Errors/builtin_error\
		Errors/show_error\
		Errors/token_error\
		Free/free\
		Free/free_utils\
		Free/multifree\
		Utils/get_prompt\
		Utils/adress_keeper\
		Utils/check_access\
		Utils/exec_utils\
		Utils/token_utils\
		Utils/token_fd_utils\
		Utils/fd_out_utils\
		Utils/fd_in_utils\
		Utils/dlist_utils\
		Utils/expand_utils\
		Utils/token_dlist_utils\
		Utils/resources
SRCS = $(addsuffix .c, $(addprefix Sources/, $(FILES)))
OBJS = $(addsuffix .o, $(addprefix Objects/, $(FILES)))

$(NAME): $(OBJS)
	$(PRINT) "\n${_YELLOW}Checking Libft...${_END}"
	$(MAKE_LIBFT)
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJS) -lreadline -o $(NAME) $(LIBFT)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.\a${_END}"

Objects/%.o: Sources/%.c Makefile $(HEADER)
	$(DIR) Objects
	$(DIR) Objects/Utils Objects/Environment Objects/Get_input Objects/Parsing Objects/Parsing/Expand Objects/Parsing/Quotes Objects/Tokenizer Objects/Tokenizer/Redirections Objects/Tokenizer/Cmd_arrays Objects/Execution Objects/Execution/Built_in Objects/Execution/Built_in/Export Objects/Execution/Built_in/Exit Objects/Signals Objects/Errors Objects/Free
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

clean:
	$(CLEAN_LIBFT)
	$(PRINT) "\n${_BOLD}Cleaning Objects...${_END}"
	$(RM) .OS
	$(RM) $(OBJS)
	$(PRINT) "${_BOLD}${_GREEN}Objects cleaned.\a${_END}"

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
	$(PRINT) "${_GREEN}Objects directory deleted.${_END}"
	$(PRINT) "${_GREEN}$(NAME) deleted.\a\n${_END}"

re: fclean all

exec : all
	./minishell

val : all
	valgrind --leak-check=full --show-reachable=no --track-origins=yes -s --track-fds=yes ./minishell

# debug: $(OBJS)
# 	$(PRINT) "${_RED}----------DEBUG----------${_END}"
# 	$(NORM)
# 	leaks_r 

.PHONY: all clean fclean re exec val
