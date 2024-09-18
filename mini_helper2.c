/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_helper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 20:44:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/17 19:31:53 by wdegraf          ###   ########.fr       */
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
