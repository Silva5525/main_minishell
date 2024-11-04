/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:27:11 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 13:20:33 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// i kill the process with SIGTERM to avoid zombies.. sometimes exit is 
/// simply not enough. xD sry (especially if i unset the PATH it maks sense)
/// @param arr needed for freeing
void	error_free_exit(t_arr *arr, char *str)
{
	pid_t	pid;

	pid = arr->pid;
	if (arr->seg)
	{
		free_seg(arr->seg, arr->seg_count + 1);
		free(arr->seg);
		arr->seg = NULL;
	}
	free_tokens(arr->arr);
	if (!str)
	{
		write(1, "exit\n", 5);
		kill(pid, SIGTERM);
		exit(arr->stat);
	}
	kill(pid, SIGTERM);
	exit(arr->stat);
}

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

/// @brief frees the whole t_arr struct and all its members and sets them
/// to NULL.
/// @param arr all data of the minishell.
void	free_tokens_seg(t_arr *arr)
{
	if (!arr)
		return ;
	if (arr->direktory)
	{
		free(arr->direktory);
		arr->direktory = NULL;
	}
	if (arr->hold)
		free_hold(arr, 0);
	if (arr->ken)
		free_ken_str(arr, 0, 0);
	if (arr->ken)
	{
		free(arr->ken);
		arr->ken = NULL;
	}
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}

/// @brief frees the segment and all its members.
/// @param seg the segment to be freed.
/// @param j count of the segment.
void	free_seg(t_arr **seg, size_t j)
{
	size_t	i;

	i = 0;
	if (!seg)
		return ;
	while (i < j + 1)
	{
		if (seg)
		{
			if (seg[i])
			{
				free_tokens_seg(seg[i]);
				seg[i] = NULL;
			}
		}
		i++;
	}
}
