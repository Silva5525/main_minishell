/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:08:14 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/20 18:29:55 by wdegraf          ###   ########.fr       */
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
		{
			write(2, "Error, ft_unsetenv in b_unset\n", 30);
			free_tokens(arr);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

// export TEST_VAR=test
// env | grep TEST_VAR	// shows TEST_VAR=test
// unset TEST_VAR
// env | grep TEST_VAR	// shows nothing
