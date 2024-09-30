/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:02:37 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/26 13:55:54 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief Handles the cleanup of the order array and the arr struct before
/// exiting the program.
/// @param order all the orders to be freed.
/// @param arr the struct with all the data to be freed.
void	mini_exit(char ***order, t_arr *arr)
{
	free_order(order);
	free_tokens(arr);
	exit(EXIT_FAILURE);
}

/// @brief Copies the arguments of pipe command into the order array.
/// @param arr holds all information about the minishell.
/// @param order double pointer to the array of strings.
/// @param hold Start index of the token array.
/// @param i Ending index exclusive of the command in the token array.
/// @return true if successful, false if malloc fails.
bool	loop_arg(t_arr *arr, char **order
		, size_t hold, size_t i)
{
	size_t	k;

	k = 0;
	while (hold < i)
	{
		order[k] = ft_strdup(arr->ken[hold]->str[0]);
		if (!order[k])
			return (write(2, "Error, ft_strdup failed in split_pipe_orders\n",
					45), false);
		k++;
		hold++;
	}
	order[k] = NULL;
	return (true);
}

/// @brief Copies the arguments of pipe command into the order array.
/// @param arr containing all the information about the minishell.
/// @param order Double pointer representing one command in the pipe.
/// @param i ending index of the command in token array.
/// @param hold starting index of the command in token array.
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

/// @brief Splits token array into multiple commands, separated by pipes.
/// It counts the number of pipes in the token array to determine the number
/// of commands. It then allocates memory for the order array in a tripl
/// pointer. It then calls split_pipe_orders_loop to copy the commands into
/// the order array.
/// @param arr holds all information about the minishell.
/// @return the triple pointer to the array of strings containing all commands
/// in the pipe.
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
		error_free_exit(arr, "Error, malloc failed in split_pipe_orders\n");
	split_pipe_orders_loop(arr, order, 0, 0);
	order[count] = NULL;
	return (order);
}

/// @brief Handles the creation of the pipe and the execution of the commands.
/// It splits the tokens into individual commands and then creates the pipes
/// and forks the processes. It then executes the commands in the child
/// processes and waits for the child processes to finish in the parent
/// process.
/// @param arr all the information about the minishell.
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
