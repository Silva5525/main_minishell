/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:08:46 by wdegraf           #+#    #+#             */
/*   Updated: 2024/07/18 18:40:11 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	b_env(t_arr *arr)
{
	size_t	i;

	i = 0;
	while (arr->envp[i])
	{
		if (arr->envp[i] == NULL)
		{
			write(2, "Error, envp is NULL in b_env\n", 30);
			return ;
		}
		printf("%s\n", arr->envp[i]);
		i++;
	}
}