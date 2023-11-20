# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/16 13:10:41 by chonorat          #+#    #+#              #
#    Updated: 2023/11/10 16:18:56 by pgouasmi         ###   ########.fr        #
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
CFLAGS = -Wall -Wextra -Werror -IIncludes
RM = @rm -rf
CC = @cc
DIR = @mkdir -p
PRINT = @echo
HEADER = Includes/minishell.h
LIBFT = Libft/libft.a
MAKE_LIBFT = @make -C Libft
CLEAN_LIBFT = @make clean -C Libft
FCLEAN_LIBFT = @make fclean -C Libft
NORM = @norminette | awk '$$NF!="OK!" {print "$(_RED)" $$0 "\033[0;0m"}'
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
		Tokenizer/Redirections/tokens_dlist\
		Tokenizer/Redirections/tokens_fd\
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
		Signals/pipe_sig\
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
		Utils/pipe_utils\
		Utils/resources
OBJS = $(addsuffix .o, $(addprefix Objects/, $(FILES)))

$(NAME): $(OBJS) $(LIBFT)
ifeq ($(OS),Linux)
	$(PRINT) "\n${_BOLD}Waiting for norminette...${_END}"
	$(NORM)
	$(PRINT) "${_BOLD}Norminette done.${_END}"
	$(PRINT) "\n${_YELLOW}Making $(NAME)...${_END}"
	$(CC) $(OBJS) -lreadline -o $(NAME) $(LIBFT)
	$(PRINT) "${_BOLD}${_GREEN}$(NAME) done.\a${_END}"
else
	$(PRINT) "$(OS)" > .OS
endif

$(LIBFT): force
ifeq ($(OS),Linux)
	$(PRINT) "\n${_YELLOW}Checking Libft...${_END}"
	$(MAKE_LIBFT)
else
	$(PRINT) "\nThis $(NAME) was made for Linux only.\a\n"
endif

Objects/%.o: Sources/%.c Makefile $(HEADER)
ifeq ($(OS),Linux)
	$(DIR) Objects Objects/Utils Objects/Environment Objects/Get_input Objects/Parsing
	$(DIR) Objects/Parsing/Expand Objects/Parsing/Quotes Objects/Tokenizer Objects/Tokenizer/Redirections
	$(DIR) Objects/Tokenizer/Cmd_arrays Objects/Execution Objects/Execution/Built_in Objects/Execution/Built_in/Export
	$(DIR) Objects/Execution/Built_in/Exit Objects/Signals Objects/Errors Objects/Free
	$(PRINT) "Compiling ${_BOLD}$<$(_END)..."
	$(CC) -c $(CFLAGS) $< -o $@
else
	$(PRINT) "$(OS)" > .OS
endif

all: $(NAME)

clean:
	$(CLEAN_LIBFT)
	$(PRINT) "\n${_BOLD}Cleaning Objects...${_END}"
	$(RM) .OS
	$(RM) $(OBJS)
	$(PRINT) "${_BOLD}${_GREEN}Objects cleaned.\a${_END}"

fclean:
	$(FCLEAN_LIBFT)
	$(PRINT) "\n${_RED}Cleaning Objects...${_END}"
	$(RM) $(OBJS)
	$(PRINT) "${_RED}Deleting Objects directory...${_END}"
	$(RM) Objects
	$(PRINT) "${_RED}Deleting $(NAME)...${_END}"
	$(RM) $(NAME)
	$(RM) .OS
	$(PRINT) "${_GREEN}Objects cleaned.${_END}"
	$(PRINT) "${_GREEN}Objects directory deleted.${_END}"
	$(PRINT) "${_GREEN}$(NAME) deleted.\a\n${_END}"

re: fclean all

norminette:
	$(PRINT) "\n${_BOLD}Waiting for norminette...${_END}"
	$(NORM)
	$(PRINT) "${_BOLD}Norminette done.${_END}"

force:

.PHONY: all clean fclean re norminette force
