/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:51:39 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	part_seg_free(t_arr *arr)
{
	if (arr->seg)
	{
		free_seg(arr->seg, arr->seg_count);
		free(arr->seg);
		arr->seg = NULL;
	}
	arr->seg_count = 0;
	free_ken_str(arr, 0, 0);
	free_hold(arr, 0);
}

void	ex_redir(t_arr **seg, t_arr *arr)
{
	size_t	i;
	int		in_fd;
	int		is_last;

	in_fd = STDIN_FILENO;
	if (!seg || !seg[0])
		return ;
	i = 0;
	if (arr->size == 0)
	{
		part_seg_free(arr);
		return ;
	}
	while (i < arr->seg_count + 1)
	{
		is_last = (i == arr->seg_count);
		if (!arr->redir && arr->seg_count == 0)
		{
			if (builtin(seg[0]) == EXIT_SUCCESS)
				i++;
			arr->redir = true;
			continue ;
		}
		else
			do_pipe(seg[i], in_fd, is_last);
		while (!seg[i]->wait)
			usleep(100);
		seg[i]->wait = false;
		i++;
	}
	arr->redir = false;
	part_seg_free(arr);
}
