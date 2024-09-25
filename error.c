/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:27:11 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/25 22:01:14 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief frees the tokens, writes an error message to stderr and exits
/// @param arr needed for freeing
void	error_free_exit(t_arr *arr, char *str)
{
	free_tokens(arr);
	perror(str);
	exit(EXIT_FAILURE);
}

/// @brief frees the t_arr->hold and sets it to NULL.
/// @param arr all data of the minishell.
/// @param i index of the t_arr->hold.
void	free_hold(t_arr *arr, size_t i)
{
	if (!arr->first_time && arr->hold)
	{
		i = 0;
		while (arr->hold[i])
		{
			free(arr->hold[i]);
			arr->hold[i] = NULL;
			i++;
		}
		free(arr->hold);
		arr->hold = NULL;
	}
}
