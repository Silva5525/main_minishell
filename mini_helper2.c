/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:44:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/20 16:13:39 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief checks if the command is an absolute or relative path.
/// If the command is an absolute or relative path, the function attempts to
/// execute the command at that path. If the command is not found in the
/// directories listed in the PATH, the function returns EXIT_FAILURE.
/// @param arr all information about the minishell.
/// @param order the command to execute.
/// @param args the arguments to the command.
void	absolute_relative(t_arr *arr, char *order, char **args)
{
	if (order[0] == '/' || order[0] == '.')
	{
		try_order(order, args, arr);
		return ;
	}
	return ;
}

void	reset_arr(t_arr *arr)
{
	free_ken_str(arr, 0, 0);
	arr->ken = malloc(sizeof(t_to *) * 16);
	if (!arr->ken)
	{
		write(2, "Error: failed to allocate memory for arr->ken\n", 45);
		exit(EXIT_FAILURE);
	}
	arr->size = 0;
	arr->max_size = 16;
}

// void	free_main_loop(t_arr *arr)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!arr)
// 		return ;
// 	if (arr->ken)
// 	{
// 		while (i < arr->size)
// 		{
// 			if (arr->ken[i])
// 			{
// 				if (arr->ken[i]->str)
// 				{
// 					free(arr->ken[i]->str);
// 					arr->ken[i]->str = NULL;
// 				}
// 				free(arr->ken[i]);
// 				arr->ken[i] = NULL;
// 			}
// 			i++;
// 		}
// 		free(arr->ken);
// 		arr->ken = NULL;
// 	}
// }

void	unclosed_quotes_handler(char *hold, char *read)
{
	hold = unclosed_quotes(read);
	free(read);
	read = hold;
}
