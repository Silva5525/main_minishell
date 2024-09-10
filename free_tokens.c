/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:51:53 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/10 18:55:25 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief frees the allocated variables in the doller_question function
/// and writes an error message if an ERROR occured.
/// @param first string for the portion before "$?" in the original string
/// @param last string for the portion after "$?" in the original string
/// @param stat_str string containing the status of the last command
/// @param er if it has to write an error message
void	free_doller_question(char *first, char *last, char *stat_str, bool er)
{
	if (first)
		free(first);
	if (last)
		free(last);
	if (stat_str)
		free(stat_str);
	if (er)
		write(2,
			"Error, ft_strndup, ft_ito or malloc in doller_question\n", 55);
}

/// @brief frees the triple pointer order for the pips and sets it to NULL.
/// @param order the triple pointer to be freed.
void	free_order(char ***order)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (order[i])
	{
		j = 0;
		while (order[i][j])
		{
			free(order[i][j]);
			order[i][j] = NULL;
			j++;
		}
		free(order[i]);
		order[i] = NULL;
		i++;
	}
	free(order);
}

/// @brief frees the t_arr->envp and sets it to NULL.
/// @param arr all data of the minishell.
/// @param i index of the t_arr->envp.
void	free_envp(t_arr *arr, size_t i)
{
	i = 0;
	if (!arr->first_time && arr->envp)
	{
		i = 0;
		while (arr->envp[i])
		{
			free(arr->envp[i]);
			arr->envp[i] = NULL;
			i++;
		}
		free(arr->envp);
		arr->envp = NULL;
	}
}

/// @brief frees the t_arr->ken->str and sets it to NULL.
/// @param arr all data of the minishell.
/// @param i index of the t_arr->ken.
/// @param j index of the t_arr->ken->str.
void	free_ken_str(t_arr *arr, size_t i, size_t j)
{
	i = 0;
	while (i < arr->size)
	{
		if (arr->ken[i])
		{
			if (arr->ken[i]->str)
			{
				j = 0;
				if (arr->ken[i]->str[j])
				{
					while (arr->ken[i]->str[j])
					{
						free(arr->ken[i]->str[j]);
						arr->ken[i]->str[j] = NULL;
						j++;
					}
				}
				free(arr->ken[i]->str);
				arr->ken[i]->str = NULL;
			}
			free(arr->ken[i]);
			arr->ken[i] = NULL;
		}
		i++;
	}
}

/// @brief frees the whole t_arr struct and all its members and sets them
/// to NULL.
/// @param arr all data of the minishell.
void	free_tokens(t_arr *arr)
{
	if (!arr)
		return ;
	if (arr->direktory)
	{
		free(arr->direktory);
		arr->direktory = NULL;
	}
	free_envp(arr, 0);
	free_ken_str(arr, 0, 0);
	if (arr->ken)
	{
		free(arr->ken);
		arr->ken = NULL;
	}
	free(arr);
}
