/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:07:29 by wdegraf           #+#    #+#             */
/*   Updated: 2024/07/17 17:00:46 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void b_pwd(t_arr *arr)
{
	if (arr->direktory)
		free(arr->direktory), arr->direktory = NULL;
	arr->direktory = getcwd(NULL, 0);
	if (arr->direktory)
		printf("%s\n", arr->direktory);
	else
		write(2, "Error, b_pwd failed\n", 20);
}