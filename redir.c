/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:58:42 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/25 22:05:53 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief closes the stdout and calls the builtin function if there is a
/// command in the tokens. If there is no command it writes an error message
/// to stderr and exits the program.
/// @param stdout restored stdout file descriptor.
/// @param arr contains all information about the minishell.
void	close_out_redir(int stdout, t_arr *arr, int fd)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		error_free_exit(arr, "Error, dup2 fail in out_redir\n");
	}
	close(fd);
	remove_redir_token(arr, arr->size - 2);
	if (arr->ken && arr->ken[0] && arr->ken[0]->str[0])
		builtin(arr);
	else
		write(2, "Error, empty command.\n", 22);
	if (dup2(stdout, STDOUT_FILENO) < 0)
	{
		close(stdout);
		error_free_exit(arr, "Error, dup2 fail in out_redir\n");
	}
	close(stdout);
}

/// @brief If append is true it opens the file in append mode, otherwise in
/// it truncates the file (means overwrites the file). Then redirects the
/// stdout (STDOUT_FILENO) to the file descriptor of the file. If it fails
/// it writes an error message to stderr and exits the program.
/// @param file Name of the file to redirect to.
/// @param append pif true, appends to file, otherwise truncates file.
/// @param arr all information about the minishell. (here needed for freeing)
void	out_redir(char *file, bool append, t_arr *arr)
{
	int	fd;
	int	stdout;

	stdout = dup(STDOUT_FILENO);
	if (stdout < 0)
		error_free_exit(arr, "Error, dup fail in out_redir\n");
	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		error_free_exit(arr, "Error, open failed in out_redir\n");
	close_out_redir(stdout, arr, fd);
}

/// @brief Opens the specified file in read only mode and redirects the
/// stdin (STDIN_FILENO) to the file descriptor of the file. If it fails
/// it writes an error message to stderr and exits the program.
/// @param file name of the file to redirect from.
/// @param arr all information about the minishell. (here needed for freeing)
void	in_redir(char *file, t_arr *arr)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_free_exit(arr, "Error, open failed in in_redir\n");
	if (dup2(fd, 0) < 0)
	{
		close(fd);
		error_free_exit(arr, "Error, dup2 failed in in_redir\n");
	}
	close(fd);
}

/// @brief creates a pipe and reads from stdin until the delimiter is found.
/// Then writes the input to the pipe. After that it redirects to read from
/// the pipe. If it fails it writes an error message to stderr and exits.
/// @param del delimiter to stop reading
/// @param arr all information about the minishell. (here needed for freeing)
void	mini_heredoc(char *del, t_arr *arr)
{
	char	*l;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
		error_free_exit(arr, "Error, pipe failed in mini_heredoc\n");
	while (1)
	{
		l = readline("> ");
		if (!l || ft_strcmp(l, del) == 0)
		{
			free(l);
			break ;
		}
		write(pipefd[1], l, ft_strlen(l));
		write(pipefd[1], "\n", 1);
		free(l);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		close(pipefd[0]);
		error_free_exit(arr, "Error, dup2 failed in mini_heredoc\n");
	}
	close(pipefd[0]);
}

/// @brief Based on the token type in the shell command, this function
/// performs input ('<') output ('>') redirection ('>>' = 'A'), 
/// piping ('|'), and heredoc('<<' = 'A').
/// @param arr all information about the minishell.
void	redir(t_arr *arr)
{
	size_t	i;

	i = 0;
	while (i < arr->size)
	{
		if (arr->ken[i]->typ == '<')
			in_redir(arr->ken[i + 1]->str[0], arr);
		else if (arr->ken[i]->typ == '>')
			out_redir(arr->ken[i + 1]->str[0], false, arr);
		else if (arr->ken[i]->typ == '|')
			do_pipe(arr);
		else if (arr->ken[i]->typ == 'A')
			out_redir(arr->ken[i + 1]->str[0], true, arr);
		else if (arr->ken[i]->typ == 'H')
			mini_heredoc(arr->ken[i + 1]->str[0], arr);
		i++;
	}
	builtin(arr);
}
