/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:58:42 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/08 19:31:14 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	out_redir(char *file, bool append, t_arr *arr)
{
	int	fd;

	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free_tokens(arr);
		write(2, "Error, open fail in out_redir\n", 30);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		free_tokens(arr);
		write(2, "Error, dup2 fail in out_redir\n", 30);
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	in_redir(char *file, t_arr *arr)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		free_tokens(arr);
		write(2, "Error, open failed in in_redir\n", 31);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, 0) < 0)
	{
		free_tokens(arr);
		write(2, "Error, dup2 failed in in_redir\n", 31);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	mini_heredoc(char *del, t_arr *arr)
{
	char	*l;
	int		pipefd[2];

	if (pipe(pipefd) < 0)
	{
		free_tokens(arr);
		write(2, "Error, pipe failed in mini_heredoc\n", 35);
		exit(EXIT_FAILURE);
	}
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
		free_tokens(arr);
		write(2, "Error, dup2 failed in mini_heredoc\n", 35);
		close(pipefd[0]);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
}

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
}
