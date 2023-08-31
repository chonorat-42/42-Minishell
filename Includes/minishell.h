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
# define REDIRECT 10
# define MAX_SIZE 100

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
# include "libft.h"

typedef struct s_envp
{
	char			*var_name;
	char			*var_cont;
	struct s_envp	*next;
	struct s_envp	*prev;
}				t_envp;

typedef struct s_tokens
{
	char 	*content;
	char	**cmd_arr;
	int  	type;
	int		position;
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
	size_t		cmd_count;
	t_tokens	*tok_lst;
	t_envp		*envp;
}		t_mshell;

void	sig_handler(void);
void	get_paths(t_mshell *shell);
size_t	count_arr_size(char **arr);
void	print_arr(char **arr);
void	print_lst(t_list *lst);
void	free_struct(t_mshell *shell);
void	free_arr(char **arr);
void 	bin_exec(t_mshell *shell, char **cmd_arr, char **envp, int fd);
void	echo_case(char *prompt, int fd);
void	env_case(t_mshell *shell, int fd);
char	*get_cmd_arguments(char *prompt);
int 	cd_case(t_mshell *shell, char *cmd);
void	pwd_case(t_mshell *shell, int fd);
int 	redirect(t_mshell *shell);
void 	struct_init(t_mshell *shell);
void	sig_handler(void);
int		tokenizer(t_mshell *shell);
void 	execution(t_mshell *shell);
void	free_arr(char **arr);
void	ft_free_tokens(t_tokens	**head);
int		find_envvar_index(char **envp, const char *str);
int		is_ws(char c);
void	unset_case(t_mshell *shell, char *str);
void	get_current_location(t_mshell *shell);
void 	manage_quotes(t_mshell *shell);
char	*get_envvar_content(char *envvar, unsigned int start);
char	*get_builtin_opt(char *str, size_t *i);
size_t 	find_char_index(char *str, int c);
void    parsing(t_mshell *shell);
int		expand(t_mshell *shell, char *cmd);
int 	are_all_quotes_closed(char *str);
char 	*get_other(char *str, size_t *i);
void    get_envp(t_mshell *shell, char **envp);
void    get_input_loop(t_mshell *shell);
void	handle_pipes(t_mshell *shell, t_tokens **temp, int fd_in, int fd_out);
void	exec_forwarding(t_tokens *temp, t_mshell *shell, int fd_in, int fd_out);
void	get_current_location(t_mshell *shell);
void	print_env(t_envp *lst);





#endif


