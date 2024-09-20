/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:17:58 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/20 16:12:25 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief When user press Ctrl+C the input will be canceled
/// and the shell will present a new prompt on a new line.
/// @param sig if sig number is SIGINT it means you pressed Ctrl+C
/// @rl_on_new_line() Prepare Readline to read the next input
/// @rl_replace_line("", 0) clears the current line
/// @rl_redisplay() redisplays the prompt on a cleared line
void	read_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/// @brief copies the environment variables to the arr->envp
/// @param arr holds all data
/// @param arr->envp the environment variables
/// @param envp the environment variables
/// @param i index of the environment variables
void	envp_copy(t_arr *arr, char **envp, size_t i)
{
	size_t	j;
	size_t	k;

	j = 0;
	k = 0;
	while (j < i)
	{
		arr->envp[j] = ft_strdup(envp[j]);
		if (!arr->envp[j])
		{
			write(2, "Error, malloc failed in alloc_envp\n", 35);
			while (k < j)
			{
				free(arr->envp[k]);
				arr->envp[k] = NULL;
				k++;
			}
			free(arr->envp);
			free_tokens(arr);
			exit(EXIT_FAILURE);
		}
		j++;
	}
	arr->envp[i] = NULL;
}

/// @brief allocates the environment variables to the arr->envp
/// @param arr holds all data
/// @param envp holds the environment variables
void	alloc_envp(t_arr *arr, char **envp)
{
	size_t	i;

	if (arr->first_time)
	{
		i = 0;
		while (envp[i])
			i++;
		arr->envp = (char **)malloc(sizeof(char *) * (i + 1));
		if (!arr->envp)
		{
			write(2, "Error, malloc failed in alloc_envp\n", 35);
			free_tokens(arr);
			exit(EXIT_FAILURE);
		}
		envp_copy(arr, envp, i);
		arr->first_time = false;
	}
}

/// @brief searches for a pipe in the tokens
/// @param arr holds all data
/// @return true if a pipe was found, else false
bool	pipe_search(t_arr *arr)
{
	size_t	i;

	i = 0;
	while (i < arr->size)
	{
		if (arr->ken[i]->typ == '|')
			return (true);
		i++;
	}
	return (false);
}

/// @brief  just a helper for do_execve.. prints an error message and exits
/// @param arr the struct with all data in minishell
/// @param order_exit the order tripple pointer with all commands to be freed
void	out_error(t_arr *arr, char ***order_exit)
{
	write(2, "Error, path_dir or ft_build_f_path failed in do_execve\n", 55);
	mini_exit(order_exit, arr);
}
