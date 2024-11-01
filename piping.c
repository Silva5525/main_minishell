/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/01 15:32:45 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief frees the parts of the arr which are not needed anymore for the next 
/// loop of the minishell.
/// @param arr main minishell struct.
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

/// @brief forks the process and executes the command. Waits for the child
/// process to finish and sets the status of the last command. Closes
/// the file descriptors if they are not the standard file descriptors.
/// @param arr 
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

/// @brief handles the command execution. If its a builtin command it will be
/// executed there if not it will be executed in do_fork.
/// @param seg the array of segments.
/// @param arr the main minishell struct.
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