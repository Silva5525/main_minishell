/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:05:15 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 13:20:53 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief exits the shell with 0 and frees the tokens. no special.
/// @param arr all information for the shell
void	b_exit(t_arr *arr)
{
	if (arr->size > 1)
	{
		if (ft_isnumeric(arr->ken[1]->str[0]))
			arr->arr->stat = ft_atoi(arr->ken[1]->str[0]);
		else
		{
			write(2, "exit: numeric argument required\n", 32);
			arr->arr->stat = 2;
		}
	}
	error_free_exit(arr->arr, NULL);
}
