/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/14 20:05:26 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			write(2, "Error, ft_strdup failed in split_pipe_orders\n", 45);
			return (false);
		}
		k++;
		hold++;
	}
	order[k] = NULL;
	return (true);
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
				write(2, "Error, ft_strdup failed in split_pipe_orders\n", 45);
				mini_exit(order, arr);
			}
			if (!loop_arg(arr, order[count_i], hold, i))
			{
				write(2, "Error, loop_arg failed in split_pipe_orders\n", 45);
				mini_exit(order, arr);
			}
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
