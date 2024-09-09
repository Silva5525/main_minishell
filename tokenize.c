/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:27:22 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/09 16:03:40 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief calls the ft_strtok_thread function to split the string
/// on the delimeter " " and saves the tokens in an array of strings,
/// in a allocated memory block, if the allocation of the array fails,
/// the function will free the allocated memory and return an error message.
/// @param str the string to split
/// @param arg the array of strings
/// @return the array of strings or NULL if the allocation fails
static char	**tokenize(char *str, char **arg)
{
	char	*tok;
	char	*save_p;
	size_t	i;

	i = 0;
	tok = ft_strtok_thread(str, " ", &save_p);
	while (tok)
	{
		arg[i] = ft_strdup(tok);
		if (!arg[i])
		{
			while (i > 0)
			{
				free(arg[i]);
				i--;
			}
			free(arg);
			return (write(2, "Error, split_str_to_arg ft_strdub\n", 35), NULL);
		}
		i++;
		tok = ft_strtok_thread(NULL, " ", &save_p);
	}
	arg[i] = NULL;
	return (arg);
}

/// @brief splits a string into an array of strings.
/// first it counts the number of words in the string, then it allocates
/// memory for the array of strings and calls the tokenize function.
/// @param str 
/// @return 
char	**split_str_to_arg(char *str)
{
	int		w_count;
	char	**arg;

	w_count = ft_strcwords_isspace(str);
	arg = malloc(sizeof(char *) * (w_count + 1));
	if (!arg)
		return (write(2, "Error, split_str_to_arg malloc\n", 32), NULL);
	return (tokenize(str, arg));
}
