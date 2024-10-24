/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:52:57 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 17:31:15 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <limits.h>
# include <linux/limits.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "libft/libft.h"

/// @brief struct for the tokens
/// @param str the token
/// @param typ the type of the tokens
typedef struct s_tokens
{
	char	**str;
	int		typ;
}	t_to;

/// @brief struct for the minishell
/// @param ken array of tokens
/// @param envp environment variables
/// @param direktory current working directory
/// @param size number of tokens
/// @param max_size maximum number of tokens before resizing arr
/// @param in_fd file descriptor for input
/// @param out_fd file descriptor for output
/// @param stat status of the last command
/// @param first_time if true envp will be set to the envp (leak handling)
/// @param stdin file descriptor for standard input (normally 0 in main_loop)
struct				s_arr;

typedef struct s_arr
{
	t_to			**ken;
	char			**envp;
	char			**hold;
	char			*direktory;
	size_t			size;
	size_t			max_size;
	int				in_fd;
	int				out_fd;
	int				stat;
	bool			first_time;
	int				stdin;
	pid_t			pid;
	size_t			seg_count;
	struct s_arr	**seg;
	struct s_arr	*arr;
	bool			wait;
	bool			redir;
}	t_arr;

/// @brief struct for the built-in functions
/// it compares the command with the built-in commands
/// if the command is a built-in command, the corresponding function is called
/// if the command is not a built-in command, the command is executed.
/// @param name the name of the built-in command
/// @param fun the function of the built-in command
typedef void		(*t_built_f)(t_arr *arr);
typedef struct s_builtin
{
	char		*name;
	t_built_f	fun;
}	t_b;

/// ########################################## @brief./built ################

void	b_echo(t_arr *arr);
void	b_cd(t_arr *arr);
void	b_pwd(t_arr *arr);
void	b_export(t_arr *arr);
void	b_unset(t_arr *arr);
void	b_env(t_arr *arr);
void	b_exit(t_arr *arr);

/// @param built/a_builtins.c

int		builtin(t_arr *arr);
void	command_not_found(t_arr *arr);

/// @param built/b_echo.c

char	*strip_quotes(const char *str);

/// @param built/ft_arr_setenv.c

char	**ft_arr_setenv(const char *str, const char *v, char **envp,
			bool first_time);

/// ########################################## @brief Sources ################

/// @param tokenize.c

char	**split_str_to_arg(char *str);
bool	is_env_token(char *str);
int		env_assign(char *read, char ***envp);
/// @param lexer.c

void	to_ken_producer(const char *read, t_arr *arr);

/// @param expand.c

char	*expanding_env(char *read, char **envp, t_arr *arr);
char	*ft_getenv_val(char **envp, char *str);

/// @param free_tokens.c

void	free_tokens(t_arr *arr);
void	free_envp(t_arr *arr, size_t i);
void	free_order(char ***order);
void	free_ken_str(t_arr *arr, size_t i, size_t j);
void	free_doller_question(char *first, char *last, char *stat_str, bool er);

/// @param redir.c

void	remove_redir_token(t_arr *arr, size_t i);

/// @param piping.c

/// @param pipe_ex.c

int		do_pipe(t_arr *arr, int in_fd, int is_last);

/// @param open_quotes.c

char	*unclosed_quotes(char *line);
bool	has_open_quotes(const char *line);

/// @param mini_helper.c

// bool	pipe_search(t_arr *arr);
void	envp_copy(t_arr *arr, char **envp, size_t i);
void	alloc_envp(t_arr *arr, char **envp);
void	read_signal(int sig);
void	absolute_relative(t_arr *arr, char *order, char **args);

/// @param mini_helper2.c

void	reset_arr(t_arr *arr, char *read);
char	*direktory_minishell(void);
char	**order_concate(t_arr *arr);

/// @param manage_token.c

char	*doller_question(char *str, int stat);
void	catch_token(t_arr *arr, t_to *ken);
t_arr	*flexible_arr(void);
t_to	*list_token(char **val, int typ);

/// @param execute.c

void	ex_order(t_arr *arr);
char	**path_dir(t_arr *arr);
int		try_order(char *full_path, char **args, t_arr *arr);

/// @param error.c

void	error_free_exit(t_arr *arr, char *str);
void	free_hold(t_arr *arr, size_t i);

void	new_to_ken_producer(t_arr *arr);
void	free_seg(t_arr **seg, size_t j);
void	ex_redir(t_arr **seg, t_arr *arr);
void	free_tokens_seg(t_arr *arr);
int		find_to_ex(char *order, char **args, t_arr *arr);

/// @param redir.c

void	redir_heardoc(t_arr *seg, t_arr *arr, const char *delimiter);
void	redir_append(t_arr *seg, t_arr *arr, const char *file);
void	redir_in(t_arr *seg, t_arr *arr, char *filename);
void	redir_out(t_arr *seg, t_arr *arr, const char *file);

#endif
