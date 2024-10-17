/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:27:11 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/17 16:36:14 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief  just a helper for do_execve.. prints an error message and exits
/// @param arr the struct with all data in minishell
/// @param order_exit the order tripple pointer with all commands to be freed
void	out_error(t_arr *arr, char ***order_exit)
{
	write(2, "Error, path_dir or ft_build_f_path failed in do_execve\n", 55);
	mini_exit(order_exit, arr);
}

/// @brief frees the tokens, writes an error message to stderr and exits.
/// i kill the process with SIGTERM to avoid zombies.. sometimes exit is 
/// simply not enough. xD sry (especially if i unset the PATH it maks sense)
/// @param arr needed for freeing
void	error_free_exit(t_arr *arr, char *str)
{
	pid_t	pid;

	pid = arr->pid;
	free_seg(arr->seg);
	free_tokens(arr);
	if (!str)
	{
		write(1, "exit\n", 5);
		kill(pid, SIGTERM);
		exit(EXIT_SUCCESS);
	}
	perror(str);
	kill(pid, SIGTERM);
	exit(EXIT_FAILURE);
}

// /// @brief frees the tokens, writes an error message to stderr and exits.
// /// @param arr needed for freeing
// void	error_free_exit(t_arr *arr, char *str)
// {
// 	pid_t	pid;

// 	pid = arr->pid;
// 	(void)pid;
// 	free_tokens(arr);
// 	if (!str)
// 	{
// 		write(1, "exit\n", 5);
// 		exit(EXIT_SUCCESS);
// 	}
// 	perror(str);
// 	exit(EXIT_FAILURE);
// }

/// @brief frees the t_arr->hold and sets it to NULL.
/// @param arr all data of the minishell.
/// @param i index of the t_arr->hold.
void	free_hold(t_arr *arr, size_t i)
{
	if (!arr->first_time && arr->hold)
	{
		i = 0;
		while (arr->hold[i])
		{
			free(arr->hold[i]);
			arr->hold[i] = NULL;
			i++;
		}
		free(arr->hold);
		arr->hold = NULL;
	}
}
