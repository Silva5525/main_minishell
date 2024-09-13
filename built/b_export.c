/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:07:54 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/11 14:55:33 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief handles the export command if no options are given.
/// @param arr all information about the shell.
static void	simple_export(t_arr *arr)
{
	size_t	i;

	i = 0;
	while (arr->envp[i])
	{
		printf("declare -x %.*s=\"%s\"\n",
			(int)(ft_strchr(arr->envp[i], '=') - arr->envp[i]),
			arr->envp[i], ft_strchr(arr->envp[i], '=') + 1);
	}
}

/// @brief Handles the "export VAR=value" case in the export command.
/// sets the environment variable to the given value.
/// @param arr the struct that holds all data.
/// @param str the variable name to be set.
/// @param tmp value to be assigned to the variable.
static void	export_value(t_arr *arr, char *str, char *tmp)
{
	char	**new_envp;

	new_envp = ft_arr_setenv(str, tmp, arr->envp, arr->first_time);
	if (!new_envp)
	{
		write(2, "Error, ft_arr_setenv in b_export\n", 33);
		free(str);
		free_tokens(arr);
		exit(EXIT_FAILURE);
	}
	arr->envp = new_envp;
}

/// @brief Handles the "export VAR" case in the export command.
/// Setting the environment variable to an empty value.
/// @param arr all information about the shell.
/// @param str variable name to be set.
static void	export_no_value(t_arr *arr, char *str)
{
	char	**new_envp;

	new_envp = ft_arr_setenv(str, "", arr->envp, arr->first_time);
	if (!new_envp)
	{
		write(2, "Error, ft_arr_setenv in b_export\n", 33);
		free(str);
		free_tokens(arr);
		exit(EXIT_FAILURE);
	}
	arr->envp = new_envp;
}

/// @brief Extracts the variable name from the given argument (before the =)
/// @param arr all information about the shell
/// @param i the index of the argument
/// @return the allocated variable name
static char	*string_name(t_arr *arr, size_t i)
{
	char	*str;

	if (!arr->ken[i]->str[0])
	{
		write(2, "Error, arr->ken[i]->str[0] in b_export\n", 38);
		free_tokens(arr);
		exit(EXIT_FAILURE);
	}
	str = ft_strndup(arr->ken[i]->str[0],
			ft_strchr(arr->ken[i]->str[0], '=') - arr->ken[i]->str[0]);
	if (!str[0])
	{
		write(2, "Error, ft_strndup in b_export\n", 31);
		free_tokens(arr);
		exit(EXIT_FAILURE);
	}
	return (str);
}

/// @brief handles the export command for setting environment variables.
/// If no arguments are given, it prints all environment variables.
/// If arguments are given, it sets the environment variables.
/// @param arr all information about the shell
void	b_export(t_arr *arr)
{
	size_t	i;
	char	*str;
	char	*tmp;

	if (arr->size == 1)
		return (simple_export(arr));
	i = 1;
	while (i < arr->size)
	{
		str = string_name(arr, i);
		tmp = ft_strchr(arr->ken[i]->str[0], '=');
		if (tmp)
			export_value(arr, str, tmp + 1);
		else
			export_no_value(arr, str);
		free(str);
		i++;
	}
}
