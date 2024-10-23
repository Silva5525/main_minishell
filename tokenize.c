/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:27:22 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/21 11:33:58 by wdegraf          ###   ########.fr       */
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

/// @brief checks if str contains a "=" and the first character is a letter.
/// So we can be sure that it is a environment variable.
/// @param str the string to check
/// @return returns true if the string is a environment variable.
bool	is_env_token(char *str)
{
	size_t	i;

	i = 0;
	if (str && ft_strncmp(str, "export", 6) == 0 && (str[6] == ' '
			|| str[6] == '\0'))
		return (false);
	if (str && ft_strchr(str, '=') && ft_isalpha(str[0]))
	{
		while (str[i] && str[i] != '=')
		{
			if (!ft_isupper(str[i]) && str[i] != '_')
				return (false);
			if (ft_isspace(str[i]))
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}

/// @brief env_assign splits the string into key (variable) and value
/// and assigns the value to the key in the environment variables.
/// @param read the string to split
/// @param envp the environment variables
int	env_assign(char *read, char ***envp)
{
	char	*key;
	char	*val;

	key = ft_strndup(read, ft_strchr(read, '=') - read);
	if (!key)
		return (write(2, "Error, env_assign ft_strndup\n", 29), EXIT_FAILURE);
	val = ft_strdup(ft_strchr(read, '=') + 1);
	if (!val)
		return (free(key), write(2, "Error, env_assign ft_strdup\n", 29),
			EXIT_FAILURE);
	*envp = ft_arr_setenv(key, val, *envp, false);
	free(key);
	free(val);
	return (EXIT_SUCCESS);
}
