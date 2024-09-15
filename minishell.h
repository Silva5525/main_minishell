/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:52:57 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/15 16:38:59 by wdegraf          ###   ########.fr       */
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
/// @param first_time if true envp will be set to the envp (leak handling)
typedef struct s_arr
{
	t_to	**ken;
	char	**envp;
	char	*direktory;
	size_t	size;
	size_t	max_size;
	int		in_fd;
	int		out_fd;
	int		stat;
	bool	first_time;
}	t_arr;

/// @brief struct for the built-in functions
/// it compares the command with the built-in commands
/// if the command is a built-in command, the corresponding function is called
/// if the command is not a built-in command, the command is executed.
/// @param name the name of the built-in command
/// @param fun the function of the built-in command
typedef void	(*t_built_f)(t_arr *arr);
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
char	**ft_arr_setenv(const char *str, const char *v,
			char **envp, bool first_time);

/// @param built/a_builtins.c

void	builtin(t_arr *arr);
void	command_not_found(void);

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

/// @param free_tokens.c

void	free_tokens(t_arr *arr);
void	free_order(char ***order);
void	free_doller_question(char *first, char *last, char *stat_str, bool er);

/// @param redir.c

void	redir(t_arr *arr);

/// @param piping.c

void	do_pipe(t_arr *arr);
void	mini_exit(char ***order, t_arr *arr);

/// @param pipe_ex.c

void	ex_pipe_order(char ***order, t_arr *arr);

/// @param open_quotes.c

char	*unclosed_quotes(char *line);
bool	has_open_quotes(const char *line);

/// @param mini_helper.c

bool	pipe_search(t_arr *arr);
void	envp_copy(t_arr *arr, char **envp, size_t i);
void	alloc_envp(t_arr *arr, char **envp);
void	read_signal(int sig);
void	out_error(t_arr *arr, char ***order_exit);

/// @param manage_token.c

char	*doller_question(char *str, int stat);
void	catch_token(t_arr *arr, t_to *ken);
t_arr	*flexible_arr(void);
t_to	*list_token(char **val, int typ);

/// @param execute.c

void	ex_order(t_arr *arr);
char	**path_dir(t_arr *arr);

#endif