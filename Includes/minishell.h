/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chonorat <chonorat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 21:31:46 by chonorat          #+#    #+#             */
/*   Updated: 2023/10/21 21:31:46 by chonorat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define CMD 1
# define INFILE 2
# define OUTFILE 3
# define ENVVAR 4
# define STRING 5
# define PIPE 6
# define LCHEVRON 7
# define RCHEVRON 8
# define APPEND 9
# define HEREDOC 10
# define HEREDEL 11

# define MAX_LL "9223372036854775807"
# define MIN_LL "9223372036854775808"

//TEMP
# define TEST 1

# ifndef DEBUG
#  define DEBUG 0
# endif
//

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef struct s_var
{
	char	*name;
	char	*content;
	int		readable;
	int		alterable;
}				t_var;

typedef struct s_envp
{
	struct s_var	var;
	struct s_envp	*next;
	struct s_envp	*prev;
}					t_envp;

typedef struct s_dlist
{
	char			*content;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}					t_dlist;

typedef struct s_tokens
{
	char			*content;
	char			**cmd_arr;
	int				fd_in;
	char			*fd_in_str;
	int				fd_out;
	char			*fd_out_str;
	int				type;
	t_dlist			*dlst;
	struct s_tokens	*prev;
	struct s_tokens	*next;
}					t_tokens;

typedef struct s_mshell
{
	char		*input;
	char		**cmd;
	size_t		envp_size;
	char		**menvp;
	char		**paths;
	char		*current_loc;
	int			exit_status;
	char		**exit;
	size_t		cmd_count;
	t_tokens	*tok_lst;
	t_envp		*envp;
	t_envp		*export;
}				t_mshell;

extern long long	g_status;

//LOOP
void		get_input_loop(t_mshell *shell);
char		*get_prompt(t_mshell *shell);
int			history(char *input);

//SIGNALS
void		default_sig(t_mshell *shell);
void		ignore_sig(t_mshell *shell);
void		heredoc_sig(t_mshell *shell, int fd);
void		exec_sig(t_mshell *shell);

//ENVIRONMENT
void		get_envp(t_mshell *shell, char **envp, char **argv);
void		create_envp(t_mshell *shell, char **envp, char **argv);
void		create_envp_list(t_mshell *shell, t_var *var);
void		update_envp(t_mshell *shell);
void		update_shlvl(t_mshell *shell);
char		*get_envvar_content(t_mshell *shell, t_envp *envp, char *to_find);
void		get_var(t_mshell *shell, char *cmd);
int			is_var(char *arg);
int			find_envvar_index(char **envp, const char *str);
void		get_svar(t_mshell *shell);
int			var_exist(t_mshell *shell, char *var);
int			delete_envvar(t_envp **envp, char *var, int ign_param);

void		get_paths(t_mshell *shell);

//KEEPER
t_mshell	*adress_keeper(t_mshell *shell);
int			*fd_keeper(int *fd);
t_dlist		**dlist_keeper(t_dlist **lst);

//PARSING
void		parsing(t_mshell *shell);
char		*remove_quotes(char	*str);
char		*get_other(char *str, size_t *i);

int			expand(t_mshell *shell, char *cmd);
char		*expand_envvar(t_mshell *shell, char *str, t_envp *envp);
int			check_after_dollar(char *str);
size_t		last_envvar_char(char *str);
int			is_char_in_set(char c, char *set);
int			are_all_quotes_closed(char *str);
void		move_to_next_quote(char *str, size_t *i, char c);

//TOKENS
int			tokenizer(t_mshell *shell);
void		parse_tkn(t_tokens **tok, t_mshell *shell);
void		split_tokens_into_dlst(t_tokens **lst, t_mshell *shell);
void		manage_quotes_arr(t_tokens	**lst);
void		create_cmd_arr(t_tokens **tk_lst, t_mshell *shell);
void		split_on_pipes(t_mshell *shell, char *str);
void		tokens_addback(t_tokens **lst, t_tokens *new);
void		init_new_token(t_tokens **new);
void		pipe_found(t_mshell *shell, char *str, size_t *i, size_t *j);
void		give_type(t_tokens **lst);
void		create_token(t_mshell *shell, int i, int j, char *to_add);

void		split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j);
char		*join_dlist(t_dlist	*lst);
void		expand_dlist(t_mshell *shell, t_envp *envp, t_dlist **lst);

//FD
void		get_fds(t_mshell *shell, t_tokens **lst);
void		get_fd_in(t_mshell *shell, t_tokens **tok);
void		manage_fd(int fd_in, int fd_out);
int			handle_fd(int fd, char *file, int type);
void		close_fd(t_mshell *shell);
void		close_all_fds(t_tokens *lst, int *old_fd);
int			has_bad_fd(t_tokens *temp);

//HEREDOC
void		heredoc(t_mshell *shell, char *delimiter, int fd_in);
void		heredoc_into_infile(t_dlist **lst);

//EXECUTION
void		execution(t_mshell *shell);
void		bin_exec(t_mshell *shell, char **cmd_arr);
void		exec_forwarding(t_tokens *temp, t_mshell *shell);
void		builtin_forwarding(t_tokens *temp, t_mshell *shell);
void		executable(t_tokens *temp, t_mshell *shell);
int			check_access(char *path);
int			is_builtin(t_tokens *temp);
char		*get_exec(char *cmd);
char		*get_path(char *cmd);

void		handle_pipes(t_mshell *shell, t_tokens *temp);

//BUILTINS
void		cd_case(t_mshell *shell, char **cmd);
void		pwd_case(t_mshell *shell, char **cmd, int fd);
void		echo_case(char **cmd, int fd);
void		env_case(t_mshell *shell, char **cmd, int fd);
void		unset_case(t_mshell *shell, char **cmd);
void		export_case(t_mshell *shell, char **cmd, int fd);
void		exit_case(t_mshell *shell, char **cmd);

void		print_export(t_envp *export, int fd);
int			create_export(t_envp **export, t_envp *envp);
void		check_plus(t_mshell *shell, char **arg);

int			check_option(t_mshell *shell, char **cmd);
long long	ll_atoi(const char *str);
int			only_digit(char *arg);
int			get_current_location(t_mshell *shell);

//ERROR
void		builtin_error(char *cmd, char *arg, int error);
void		show_error(char *cmd, char *type, int error);

//FREE
void		free_arr(char **arr);
void		free_tokens(t_tokens **head);
void		free_envp(t_envp **head);
void		free_tokens_dlist(t_tokens **lst);
void		free_dlist(t_dlist **head);
void		free_struct(t_mshell *shell);
void		free_arr(char **arr);
void		free_export(t_envp **export);
void		multifree(void *ptr1, void *ptr2, void *ptr3, void *ptr4);

//UTILS
long int	find_char_index(char *str, int c);
void		get_fork_status(void);

//!!DEBUG!!//
void		print_arr(char **arr);
void		print_lst(t_list *lst);
void		print_single_token(t_tokens *tkn);
void		print_tkns_down(t_tokens *lst);
void		print_dlist(t_dlist	*lst);

#endif
