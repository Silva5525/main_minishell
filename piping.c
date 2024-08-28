/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/08/20 14:20:17 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_order(char ***order)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (order[i])
	{
		j = 0;
		while (order[i][j])
		{
			free(order[i][j]);
			order[i][j] = NULL;
			j++;
		}
		free(order[i]);
		order[i] = NULL;
		i++;
	}
	free(order);
}

void	mini_exit(char ***order, t_arr *arr)
{
	free_order(order);
	free_tokens(arr);
	exit(EXIT_FAILURE);
}

bool	loop_arg(t_arr *arr, char **order
		, size_t hold, size_t i)
{
	size_t	k;

	k = 0;
	while (hold < i)
	{
		order[k] = ft_strdup(arr->ken[hold]->str[0]);
		if (!order[k])
		{
			write(2, "Error, malloc or ft_strdup failed in split_pipe_orders\n", 55);
			return false;
		}
		k++;
		hold++;
	}
	order[k] = NULL;
	return true;
}

void	split_pipe_orders_loop(t_arr *arr, char ***order, size_t i, size_t hold)
{
	size_t	count_i;

	count_i = 0;
	while (i <= arr->size)
	{
		if (i == arr->size || ft_strcmp(arr->ken[i]->str[0], "|") == 0)
		{
			order[count_i] = (char **)malloc(sizeof(char *) * (i - hold + 1));
			if (!order[count_i])
			{
				write(2, "Error, malloc or ft_strdup failed in split_pipe_orders\n", 55);
				mini_exit(order, arr);
			}
			if (!loop_arg(arr, order[count_i], hold, i))
				mini_exit(order, arr); // potential memory leaks, probably we need to free all partially allocated memory before we go ahead with mini_exit?
			hold = i + 1;
			count_i++;
		}
		i++;
	}
}

char	***split_pipe_orders(t_arr *arr)
{
	char	***order;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i < arr->size)
	{
		if (strcmp(arr->ken[i]->str[0], "|") == 0)
			count++;
		i++;
	}
	count++;
	order = (char ***)malloc(sizeof(char **) * (count + 1));
	if (!order)
	{
		write(2, "Error, malloc failed in split_pipe_orders\n", 42);
		free_tokens(arr);
		exit(EXIT_FAILURE);
	}
	split_pipe_orders_loop(arr, order, 0, 0);
	order[count] = NULL;
	return (order);
}

void	ex_pipe_order(char ***order, t_arr *arr)
{
	pid_t	pid;
	int		pipefd[2];
	int		fd;
	size_t	i;

	i = 0;
	fd = 0;
	while (order[i])
	{
		if (pipe(pipefd) < 0)
		{
			write(2, "Error, pipe failed in ex_pipe_order\n", 36);
			mini_exit(order, arr);
		}
		pid = fork();
		if (pid < 0)
		{
			write(2, "Error, fork failed in ex_pipe_order\n", 36);
			mini_exit(order, arr);
		}
		if (pid == 0)
		{
			if (order[i + 1])
				dup2(pipefd[1], STDOUT_FILENO);
			if (i != 0)
				dup2(fd, STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			//Probably we need to check if the command exists and is executable here?
			execve(order[i][0], order[i], arr->envp);
			write(2, "Error, execve failed in ex_pipe_order\n", 39);
			mini_exit(order, arr);
		}
		waitpid(pid, NULL, 0);
		close(pipefd[1]);
		fd = pipefd[0];
		i++;
	}
}

void	do_pipe(t_arr *arr)
{
	char	***order;

	order = split_pipe_orders(arr);
	if (order)
	{
		ex_pipe_order(order, arr);
		free_order(order);
	}
}
//multiple commands check?
//empty commands check?
