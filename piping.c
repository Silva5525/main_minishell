/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/01 12:35:07 by wdegraf          ###   ########.fr       */
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
	if (arr->ken)
	{
		free(arr->ken);
		arr->ken = NULL;
	}
	free_hold(arr, 0);
}

void	ex_redir(t_arr **seg, t_arr *arr)
{
	size_t	i;

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
		if (arr->seg_count == 0)
		{
			if (builtin(seg[0]) == EXIT_SUCCESS)
				i++;
			continue ;
		}
		else
			do_fork(seg[i]);
		i++;
	}
	part_seg_free(arr);
}

void	do_fork(t_arr *arr)
{
	pid_t	pid;
	int		stat;

	pid = fork();
	if (pid < 0)
		error_free_exit(arr, "Error, fork failed in do_fork\n");
	else if (pid == 0)
		ex_order(arr);
	waitpid(pid, &stat, 0);
	if (WIFEXITED(stat))
		arr->stat = WEXITSTATUS(stat);
	else if (WIFSIGNALED(stat))
		arr->stat = 128 + WTERMSIG(stat);
	if (arr->in_fd != STDIN_FILENO)
		close(arr->in_fd);
	if (arr->out_fd != STDOUT_FILENO)
		close(arr->out_fd);
}
