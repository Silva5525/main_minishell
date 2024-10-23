/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:01:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:50:03 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_pipe(t_arr *arr, int in_fd, int is_last)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!is_last && pipe(pipe_fd) < 0)
		error_free_exit(arr, "Error, pipe failed in do_pipe\n");
	pid = fork();
	if (pid < 0)
		error_free_exit(arr, "Error, fork failed in do_pipe\n");
	else if (pid == 0)
	{
		if (arr->in_fd != STDIN_FILENO)
		{
			if (dup2(arr->in_fd, STDIN_FILENO) < 0)
				error_free_exit(arr, "Error, 1dup2 in do_pipe\n");
			close(arr->in_fd);
		}
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) < 0)
				error_free_exit(arr, "Error, 2dup2 in do_pipe\n");
			close(in_fd);
		}
		if (!is_last)
		{
			close(pipe_fd[0]);
			if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
				error_free_exit(arr, "Error, 3dup2 in do_pipe\n");
			close(pipe_fd[1]);
		}
		else if (arr->out_fd != STDOUT_FILENO)
		{
			if (dup2(arr->out_fd, STDOUT_FILENO) < 0)
				error_free_exit(arr, "Error, 4dup2 in do_pipe\n");
			close(arr->out_fd);
		}
		ex_order(arr);
		exit(EXIT_SUCCESS);
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (!is_last)
	{
		close(pipe_fd[1]);
		in_fd = pipe_fd[0];
	}
	waitpid(pid, NULL, 0);
	arr->wait = true;
	if (is_last && arr->out_fd != STDOUT_FILENO)
		close(arr->out_fd);
	return (in_fd);
}
