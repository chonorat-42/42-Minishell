/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgouasmi <pgouasmi@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:52:08 by pgouasmi          #+#    #+#             */
/*   Updated: 2023/08/19 23:12:09 by pgouasmi         ###   ########.fr       */
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
# define SIZE_MAX 65535

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include "libft.h"

typedef struct s_dlist
{
	char			*content;
	struct s_d_list	*next;
	struct s_d_list	*prev;
}				t_dlist;

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
	char		*prompt;
	char		**cmd;
	size_t		envp_size;
	char		**menvp;
	char		**paths;
	char		current_loc[32767];
	size_t		cmd_count;
	t_dlist		 history;
	t_tokens	*tok_lst;
}		t_mshell;

int		get_paths(t_mshell *shell, char **envp);
size_t	count_arr_size(char **arr);
void	print_arr(char **arr);
void	print_lst(t_list *lst);
void	free_struct(t_mshell *shell);
void	free_arr(char **arr);
int 	bin_exec(t_mshell shell, char **cmd_arr, char **envp, int fd);
int		echo_case(char *prompt, int fd);
void	env_case(t_mshell *shell);
char	*get_cmd_arguments(char *prompt);
int 	cd_case(t_mshell *shell);
void	pwd_case(t_mshell *shell);
int 	redirect(t_mshell *shell);
void 	struct_init(t_mshell *shell);
void	sig_handler(void);
int		tokenizer(t_mshell *shell);
void 	execution(t_mshell *shell, char **envp);
void	free_arr(char **arr);
void	ft_free_tokens(t_tokens	**head);
int		find_envvar_index(char **envp, const char *str);
int		is_ws(char c);
void	unset_case(t_mshell *shell, char *str);
void	get_current_location(t_mshell *shell);


#endif


