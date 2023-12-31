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

//TOKENS
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
# define NO_FILE 12
# define PERMISSIONS 13
# define ARGV 14

//ERRORS
# define EXEC 1
# define QUOTES 2
# define HDOC 3
# define NO_F 4
# define PERM 5
# define SYNTAX 6
# define OPERATOR 7
# define SHLVL 8
# define CMD_NF 9
# define NSFOD 10
# define ISADIR 11

# define MAX_LL "9223372036854775807"
# define MIN_LL "9223372036854775808"

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
	int				exp;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}					t_dlist;

typedef struct s_error
{
	char			*content;
	int				type;
	struct s_error	*next;
}					t_error;

typedef struct s_pipe
{
	int	*lpids;
	int	lpids_freed;
	int	fd[2][2];
}				t_pipe;

typedef struct s_tokens
{
	char			*content;
	char			**cmd_arr;
	int				fd_in;
	char			*fd_in_str;
	int				fd_out;
	char			*fd_out_str;
	int				type;
	int				has_bad_fd;
	int				is_piped;
	t_dlist			*dlst;
	t_error			*errors;
	struct s_tokens	*prev;
	struct s_tokens	*next;
}					t_tokens;

typedef struct s_fdhandler
{
	t_tokens		*tok;
	t_dlist			*dlist;
	struct s_mshell	*shell;
	char			*fd_str;
	int				has_fd;
	int				temp_fd;
}					t_fdhandler;

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
	t_fdhandler	handler;
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
void		pipe_sig(t_mshell *shell);

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
void		*adress_keeper(void *adress);
t_mshell	*shell_keeper(t_mshell *shell);
int			*fd_keeper(int *fd);
t_dlist		**dlist_keeper(t_dlist **lst);

//PARSING
void		parsing(t_mshell *shell);
char		*remove_quotes(char	*str);
char		*get_other(char *str, size_t *i);

int			expand(t_mshell *shell, char *cmd);
char		*expand_envvar(t_mshell *shell, char *str, t_envp *envp);
void		envvar_flag(size_t index[2], size_t qcount[2], t_dlist **lst,
				char *str);
int			check_after_dollar(char *str);
size_t		last_envvar_char(char *str);
int			is_char_in_set(char c, char *set);
int			are_all_quotes_closed(char *str);
void		move_to_next_quote(char *str, size_t *i, char c);
void		expand_dlst(t_mshell *shell, t_dlist **lst, t_envp *envp);

//TOKENS
int			tokenizer(t_mshell *shell);
void		parse_tkn(t_tokens **tok, t_mshell *shell);
void		split_tokens_into_dlst(t_tokens **lst, t_mshell *shell);
int			manage_quotes_arr(t_tokens **lst);
void		create_cmd_arr(t_tokens **tk_lst, t_mshell *shell);
void		split_on_pipes(t_mshell *shell, char *str);
void		tokens_addback(t_tokens **lst, t_tokens *new);
void		init_new_token(t_tokens *new);
void		pipe_found(t_mshell *shell, char *str, size_t *i, size_t *j);
void		give_type(t_tokens **lst);
void		create_token(t_mshell *shell, int i, int j, char *to_add);

int			split_into_dlst(t_dlist **lst, char *str, size_t i, size_t j);
char		*join_dlist(t_dlist	*lst);
void		expand_dlist(t_mshell *shell, t_envp *envp, t_dlist **lst);
size_t		dlst_size(t_dlist *lst);
char		**list_into_arr(t_mshell *shell, t_dlist *lst);
void		get_chevrons(char *str, size_t *i, char c, t_dlist **lst);
void		move_to_last_quote(char *str, size_t *i, char c);
void		last_quote_redir(char *str, t_dlist **lst, size_t *i, size_t *j);

//FD
void		get_fds(t_mshell *shell, t_tokens **lst);
void		get_fd_in(t_mshell *shell, t_tokens **tok);
void		manage_fd(int fd_in, int fd_out);
int			handle_fd(int fd, char *file, int type, t_tokens *temp);
void		close_fd(t_mshell *shell);
void		close_all_fds(t_tokens *lst, int *old_fd, int *new_fd);
void		close_fd_final(t_tokens *lst);
void		close_std_fds(void);
int			has_bad_fd(t_tokens *temp);
int			empty_node_found(t_tokens *temp_t, t_dlist *temp_d);
int			handle_simple_out(t_fdhandler *handler);
int			handle_append(t_fdhandler *handler);
int			handle_simple_in(t_fdhandler *handler);
void		handle_heredoc(t_fdhandler *handler);
void		init_fdhandler(t_fdhandler *handler, t_mshell *shell,
				t_tokens *tokens);
void		init_handler_loop(t_fdhandler *handler, int type);
void		handler_end_loop(t_fdhandler *handler, int type);
int			fdin_access(char *path);
int			fdout_access(char *path);
void		bad_fd(t_fdhandler *handler);

//HEREDOC
void		heredoc(t_mshell *shell, char **delimiter, int fd_in);
int			heredoc_into_infile(t_dlist **lst);
void		hd_delim_quotes(t_mshell *shell, char **new_del, char **delim,
				int *del_quotes);

//EXECUTION
void		execution(t_mshell *shell);
void		bin_exec(t_mshell *shell, char **cmd_arr, int *pid);
void		exec_forwarding(t_tokens *temp, t_mshell *shell);
void		builtin_forwarding(t_tokens *temp, t_mshell *shell);
void		executable(t_tokens *temp, t_mshell *shell);
int			is_builtin(t_tokens *temp);
char		*get_exec(char *cmd);
char		*get_path(char *cmd);
int			check_access(char *path);

void		handle_pipes(t_mshell *shell, t_tokens *temp);
void		builtin_forwarding_pipe(t_tokens *temp, t_mshell *shell);
int			cmd_has_pipes(t_tokens *lst);
int			has_bad_fd(t_tokens *temp);
size_t		count_successive_pipes(t_tokens *temp);
void		close_all_fds_parent(t_tokens *lst, t_pipe *data);
void		close_all_fds_child(t_tokens *lst, t_pipe *data);

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
void		show_error(char *cmd, int type, int error);

void		add_error(char *file, int type, t_tokens *temp);
void		print_errors(t_tokens *lst);
void		print_errors_single(t_tokens *token);

//FREE
void		free_arr(char **arr);
void		free_tokens(t_tokens **head);
void		free_envp(t_envp **head);
void		free_tokens_dlist(t_tokens **lst);
void		free_dlist(t_dlist **head);
void		free_struct(t_mshell *shell);
void		free_arr(char **arr);
void		free_export(t_envp **export);
void		free_errors(t_error *error);
void		free_handler(t_fdhandler *handler);
void		multifree(void *ptr1, void *ptr2, void *ptr3, void *ptr4);
void		remove_hdoc(t_mshell *shell);

//UTILS
long int	find_char_index(char *str, int c);
void		get_fork_status(void);
int			is_between_simple(char *str);
void		add_flag_expand(t_dlist *lst, int to_expand);
int			str_isws(char *str);

#endif
