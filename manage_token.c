/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 19:02:33 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/26 15:36:16 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief concatenates the strings before and after "$?" with the status
/// into the allocated out string.
/// @param out filal output string.
/// @param first string before "$?".
/// @param last string after "$?".
/// @param stat_str stat of the last command.
/// @return out or NULL if the allocation fails.
static char	*split_doller_question(char *out, char *first,
				char *last, char *stat_str)
{
	if (!out)
		return (free_doller_question(
				first, last, stat_str, true), NULL);
	ft_strcpy(out, first);
	ft_strcat(out, stat_str);
	ft_strcat(out, last);
	free_doller_question(first, last, stat_str, false);
	return (out);
}

/// @brief if the string contains "$?" it will be replaced with the status
/// of the last command. The new string will be saved in a new allocated
/// memory block. If the allocation fails, the function will return an error
/// message and NULL.
/// @param str the string to be checked.
/// @param stat the status of the last command.
/// @return the new string or NULL if the allocation fails.
char	*doller_question(char *str, int stat)
{
	char	*out;
	char	*pos;
	char	*first;
	char	*last;
	char	*stat_str;

	pos = ft_strstr(str, "$?");
	if (pos)
	{
		first = ft_strndup(str, pos - str);
		if (!first)
			return (free_doller_question(NULL, NULL, NULL, true), NULL);
		last = ft_strdup(pos + 2);
		if (!last)
			return (free_doller_question(first, NULL, NULL, true), NULL);
		stat_str = ft_itoa(stat);
		if (!stat_str)
			return (free_doller_question(first, last, NULL, true), NULL);
		out = malloc(ft_strlen(first) + ft_strlen(stat_str)
				+ ft_strlen(last) + 1);
		out = split_doller_question(out, first, last, stat_str);
	}
	else
		return (ft_strdup(str));
	return (out);
}

/// @brief checks if the array of tokens is full and reallocates memory
/// if necessary. If the reallocation fails, the function will free the
/// allocated memory and return an error message and exit the program.
/// @param arr the array of tokens
/// @param ken the token to be added
void	catch_token(t_arr *arr, t_to *ken)
{
	t_to	**new_ken;

	if (arr->size == arr->max_size)
	{
		arr->max_size *= 2;
		new_ken = ft_realloc(arr->ken, sizeof(t_to *)
				* arr->max_size / 2, sizeof(t_to *) * arr->max_size);
		if (!new_ken)
			error_free_exit(arr, "Error, realloc in catch_token\n");
		arr->ken = new_ken;
	}
	arr->ken[arr->size++] = ken;
}

/// @brief creates the t_arr struct and allocates memory for the array of
/// tokens. If the allocation fails, the function will free the allocated
/// memory and return an error message.
/// @return the t_arr struct or NULL if the allocation fails.
t_arr	*flexible_arr(void)
{
	t_arr	*arr;

	arr = malloc(sizeof(t_arr));
	if (!arr)
		return (write(2, "Error, flexible_arr malloc\n", 27), NULL);
	arr->ken = malloc(sizeof(t_to) * 16);
	if (!arr->ken)
		return (free_tokens(arr), write(2,
				"Error, flexible_arr malloc\n", 27), NULL);
	arr->size = 0;
	arr->max_size = 16;
	arr->direktory = NULL;
	arr->envp = NULL;
	arr->hold = NULL;
	arr->in_fd = STDIN_FILENO;
	arr->out_fd = STDOUT_FILENO;
	arr->stat = 0;
	arr->first_time = true;
	arr->stdin = dup(STDIN_FILENO);
	if (arr->stdin < 0)
		return (free_tokens(arr), write(2,
				"Error, dup fail in flexible_arr\n", 32), NULL);
	return (arr);
}

/// @brief lists the tokens in a t_to struct and allocates memory for it.
/// If the allocation fails, the function will return an error message and NULL.
/// @param val the array of strings to be tokenized.
/// @param typ the type of the token.
/// @return the token struct or NULL if the allocation fails.
t_to	*list_token(char **val, int typ)
{
	t_to	*ken;

	ken = malloc(sizeof(t_to));
	if (!ken)
		return (NULL);
	ken->str = val;
	ken->typ = typ;
	return (ken);
}
