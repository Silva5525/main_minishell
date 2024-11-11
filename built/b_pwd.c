/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:07:29 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/11 11:47:19 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief handles the pwd command.
/// saves the current working directory to the arr->direktory and
/// prints it to the stdout.
/// @param arr all data of the minishell.
void	b_pwd(t_arr *arr)
{
	if (arr->direktory)
	{
		free(arr->direktory);
		arr->direktory = NULL;
	}
	arr->direktory = getcwd(NULL, 0);
	if (arr->direktory)
	{
		printf("%s\n", arr->direktory);
		arr->arr->stat = 0;
	}
	else
	{
		write(2, "Error, b_pwd failed\n", 20);
		arr->arr->stat = 1;
	}
}
