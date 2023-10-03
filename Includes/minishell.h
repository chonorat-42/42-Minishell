/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:52:08 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/24 19:06:24 by pgouasmi         ###   ########.fr       */
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
# define MAX_SIZE 100

# define MAX_LL "9223372036854775807"
# define MIN_LL "9223372036854775808"

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
}				t_envp;

typedef struct s_dlist
{
	char			*content;
	struct s_dlist	*prev;
	struct s_dlist	*next;
}			t_dlist;

typedef struct s_tokens
{
	char 	*content;
	char	**cmd_arr;
	int		fd_in;
	int		fd_out;
	int  	type;
	int		position;
	t_dlist	*dlst;
	struct s_tokens *prev;
	struct s_tokens *next;
}			t_tokens;

typedef struct s_mshell
{
	char		*input;
	char		**cmd;
	size_t		envp_size;
	char		**menvp;
	char		**paths;
	char		*current_loc;
	int			exit_status;
	size_t		cmd_count;
	t_tokens	*tok_lst;
	t_envp		*envp;
	t_envp		*export;
}				t_mshell;

void		sig_handler(void);
void		get_paths(t_mshell *shell);
size_t		count_arr_size(char **arr);
void		print_arr(char **arr);
void		print_lst(t_list *lst);
void		free_struct(t_mshell *shell);
void		free_arr(char **arr);
void		bin_exec(t_mshell *shell, char **cmd_arr, int fd_in, int fd_out);
int 		redirect(t_mshell *shell);
void 		struct_init(t_mshell *shell);
void		sig_handler(void);
int			tokenizer(t_mshell *shell);

void		cd_case(t_mshell *shell, char **cmd);
void		pwd_case(t_mshell *shell, int fd);
void		echo_case(char **cmd, int fd);
void		env_case(t_mshell *shell, char **cmd, int fd);
void		unset_case(t_mshell *shell, char **cmd);
void		export_case(t_mshell *shell, char **cmd, int fd);
void		exit_case(t_mshell *shell, char **cmd);

void		execution(t_mshell *shell);
int			get_final_out(t_tokens *lst);

void		free_arr(char **arr);
void		ft_free_tokens(t_tokens	**head);
int			find_envvar_index(char **envp, const char *str);
int			is_ws(char c);
void		get_current_location(t_mshell *shell);
void		manage_quotes(t_tokens **lst);
char		*get_envvar_content(t_envp *envp, char *to_find);
char		*get_builtin_opt(char *str, size_t *i);
long int	find_char_index(char *str, int c);
void		parsing(t_mshell *shell);

int			expand(t_mshell *shell, char *cmd);
char		*expand_envvar(char *str, t_envp *envp);
int			check_after_dollar(char *str);

int			are_all_quotes_closed(char *str);
char 		*get_other(char *str, size_t *i);
void		get_envp(t_mshell *shell, char **envp);
void		get_input_loop(t_mshell *shell);
void		handle_pipes(t_mshell *shell, t_tokens **temp, int fd_in, int fd_out);
void		exec_forwarding(t_tokens *temp, t_mshell *shell);
void		get_current_location(t_mshell *shell);
void		print_env(t_envp *lst);
void		create_envp_list(t_mshell *shell, t_var *var);
int			delete_envvar(t_envp **envp, char *var, int ign_param);
int			is_char_in_set(char c, char *set);

void		update_envp(t_mshell *shell);
void		free_envp(t_envp **head);

void		print_export(t_envp *export, int fd);
int			create_export(t_envp **export, t_envp *envp);
void		free_export(t_envp **export);
int			var_exist(t_mshell *shell, char *var);

long long	ll_atoi(const char *str);
int			only_digit(char *arg);

void		builtin_error(char *cmd, char *arg, int error);
void		show_error(char *cmd, char *type, int error);

void		get_svar(t_mshell *shell);

void		heredoc(char *delimiter, int fd_in, t_envp *envp);

void		print_tkns_down(t_tokens *lst);

char		*remove_quotes(char	*str);

void		free_dlist(t_dlist **head);

void		print_single_token(t_tokens *tkn);


#endif
