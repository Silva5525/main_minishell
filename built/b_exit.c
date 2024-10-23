/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:05:15 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/19 13:26:42 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief exits the shell with 0 and frees the tokens. no special.
/// @param arr all information for the shell
void	b_exit(t_arr *arr)
{
	error_free_exit(arr->arr, NULL);
}
