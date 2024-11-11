/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:08:14 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/11 11:47:25 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief removes the environment variables or shows an error message if fails.
/// @param arr all the information needed for the shell.
void	b_unset(t_arr *arr)
{
	size_t	i;

	i = 1;
	while (i < arr->size)
	{
		if (ft_unsetenv(arr->ken[i]->str[0], arr->envp) == -1)
			error_free_exit(arr, "Error, unsetenv failed in b_unset\n");
		i++;
	}
	arr->arr->stat = 0;
}
