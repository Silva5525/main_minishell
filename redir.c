/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:58:42 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/02 14:12:43 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Handles output redirection (>) by opening the given file and setting
/// the out_fd for the current segment.
/// @param seg The current segment where the out_fd is to be set.
/// @param arr The main structure holding all shell data.
/// @param file The name of the file to which output will be redirected.
void	redir_out(t_arr *seg, t_arr *arr, const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		error_free_exit(arr, "Error, open failed in redir_out");
	if (seg->out_fd != STDOUT_FILENO)
		close(seg->out_fd);
	printf("redir_out fd: %d\n", fd); // debug
	seg->out_fd = fd;
}

/// @brief Prepares input redirection for the given segment. 
/// Opens the input file and stores the file descriptor for later use.
/// @param seg Segment where redirection is being prepared.
/// @param arr Main minishell data structure (for access to env and globals).
/// @param filename The name of the file to redirect input from.
void	redir_in(t_arr *seg, t_arr *arr, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_free_exit(arr, "Error, open failed in redir_in");
	if (seg->in_fd != STDIN_FILENO)
		close(seg->in_fd);
		printf("redir_out fd: %d\n", fd); // debug
	seg->in_fd = fd;
}

/// @brief Handles output redirection in append mode. This stores the file
/// descriptor into arr->out_fd for future use but does not execute the command.
/// @param seg The segment where the command's redirection is being stored.
/// @param arr The current minishell data structure.
/// @param file The file name to redirect output to.
void	redir_append(t_arr *seg, t_arr *arr, const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		error_free_exit(arr, "Failed to open file for append");
	if (seg->out_fd != STDOUT_FILENO)
		close(seg->out_fd);
	seg->out_fd = fd;
}

/// @brief creates a here-document (<<) redirection.
/// It reads input from the user until the delimiter is found
/// and then stores it in the pipe. The input file descriptor is
/// saved in the segment for later execution.
/// @param seg current segment holding the here-doc.
/// @param arr the main shell structure.
/// @param delimiter the string that ends the here-document.
void	redir_heardoc(t_arr *seg, t_arr *arr, const char *delimiter)
{
	int		p_fd[2];
	char	*l;

	if (pipe(p_fd) < 0)
		error_free_exit(arr, "Error: Pipe failed in redir_heardoc");
	while (1)
	{
		l = readline("> ");
		if (!l || strcmp(l, delimiter) == 0)
		{
			if (l)
				free(l);
			break ;
		}
		write(p_fd[1], l, strlen(l));
		write(p_fd[1], "\n", 1);
		free(l);
	}
	close(p_fd[1]);
	if (seg->in_fd != STDIN_FILENO)
		close(seg->in_fd);
	seg->in_fd = p_fd[0];
}
