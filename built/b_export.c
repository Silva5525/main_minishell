/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:07:54 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/11 11:47:09 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief if the ft_strjoin or malloc fails, it will free the tokens and exit
/// the program. 1 are for hold and new_val, 2 is for new_envp.
/// @param new_val only free if new_envp after malloc is NULL
/// @param arr the struct that holds all data.
/// @param i the number which case should be executed.
void	export_val_fail(char *new_val, t_arr *arr, int i)
{
	if (i == 1)
		error_free_exit(arr, "Error, strjoin failed in export_value\n");
	if (i == 2)
	{
		free(new_val);
		error_free_exit(arr, "Error, malloc failed in export_value\n");
	}
}

/// @brief Helper function of export value. 
/// If the environment variable is not found, it will be allocated and
/// added to the environment variables.
/// dont forget that arr here is in real one of the seg so..
	// arr->envp = new_envp; is seg
	// arr->arr->envp = new_envp; is arr xD
/// @param new_envp the new environment variables.
/// @param new_val the new value to be added.
/// @param arr the struct that holds all data.
/// @param i the index of the environment variables.
void	new_envp_export_val(char **new_envp, char *new_val,
			t_arr *arr, size_t i)
{
	new_envp = ft_realloc(arr->envp, sizeof(char *)
			* (i + 1), sizeof(char *) * (i + 2));
	if (!new_envp)
		export_val_fail(new_val, arr, 2);
	new_envp[i] = new_val;
	new_envp[i + 1] = NULL;
	arr->envp = new_envp;
	arr->arr->envp = new_envp;
}

/// @brief Adds or updates an environment variable with value.
/// @param arr the struct that holds all data.
/// @param str the variable name to be set.
/// @param value the value to be assigned to the variable.
static void	export_value(t_arr *arr, char *str, char *value)
{
	char	*new_val;
	char	*hold;
	size_t	i;

	i = 0;
	hold = ft_strjoin(str, "=");
	if (!hold)
		export_val_fail(NULL, arr, 1);
	new_val = ft_strjoin(hold, value);
	free(hold);
	if (!new_val)
		export_val_fail(NULL, arr, 1);
	while (arr->envp[i])
	{
		if (ft_strncmp(arr->envp[i], str, ft_strlen(str)) == 0
			&& arr->envp[i][ft_strlen(str)] == '=')
		{
			free(arr->envp[i]);
			arr->envp[i] = new_val;
			return ;
		}
		i++;
	}
	new_envp_export_val(NULL, new_val, arr, i);
}

/// @brief Extracts the variable name from the given argument (before the =)
/// @param arr all information about the shell
/// @param i the index of the argument
/// @return the allocated variable name
static char	*string_name(t_arr *arr, size_t i)
{
	char	*str;
	size_t	j;

	j = 0;
	if (!arr->ken[i]->str[0])
		error_free_exit(arr, "Error, ken str in b_export\n");
	str = ft_strndup(arr->ken[i]->str[0],
			ft_strchr(arr->ken[i]->str[0], '=') - arr->ken[i]->str[0]);
	if (!str[0])
		error_free_exit(arr, "Error, strndup in b_export\n");
	while (arr->ken[i]->str[0][j] && arr->ken[i]->str[0][j] != '=')
		j++;
	if (arr->ken[i]->str[0][j] != '=')
		return (free(str), NULL);
	j = 0;
	while (str[j])
	{
		if (!ft_isupper(str[j]) && str[j] != '_')
			return (printf("why str %s \n", str), free(str), NULL);
		j++;
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
	char	*hold;

	i = 0;
	if (arr->size == 1)
		while (arr->envp[i])
			printf("%s\n", arr->envp[i++]);
	i = 1;
	while (i < arr->size)
	{
		str = string_name(arr, i);
		if (!str)
			return ;
		hold = ft_strchr(arr->ken[i]->str[0], '=');
		if (hold)
			export_value(arr, str, hold + 1);
		else
			export_value(arr, str, "");
		free(str);
		i++;
	}
	arr->arr->stat = 0;
}
