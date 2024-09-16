/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_setenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:28:56 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/16 17:34:49 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief searches for the variable in the envp array.
/// @param str the name of the variable to search for.
/// @param envp the environment variables.
/// @param j the index of the variable.
/// @return the variable or NULL if the variable was not found.
char	*serch_variable(const char *str, char **envp, size_t *j)
{
	size_t	i;
	size_t	len;

	if (!str || ft_strchr(str, '=') != NULL || *str == '\0')
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, len) == 0 && envp[i][len] == '=')
		{
			*j = i;
			return (envp[i]);
		}
		i++;
	}
	*j = i;
	return (NULL);
}

/// @brief creates a new variable with the format "str=v"
/// @param str the name of the variable.
/// @param v the value of the variable.
/// @return new variable or NULL if an error occurred.
char	*create_variable(const char *str, const char *v)
{
	char	*new;
	char	*hold;

	hold = ft_strjoin(str, "=");
	if (!hold)
		return (NULL);
	new = ft_strjoin(hold, v);
	if (!new)
		return (free(hold), NULL);
	free(hold);
	return (new);
}

/// @brief updates the envp array with the new variable.
/// @param envp environment variables.
/// @param new the new variable.
/// @param i index of the variable.
/// @param first_time bool if true envp will not be freed.
/// @return new envp array or NULL if an error occurred.
char	**update_envp(char **envp, char *new, size_t i, bool first_time)
{
	if (!envp[i])
	{
		if (!first_time)
		{
			envp = ft_realloc(envp, sizeof(char *)
					* (i + 1), sizeof(char *) * (i + 2));
			if (!envp)
				return (free(new), NULL);
		}
		envp[i + 1] = NULL;
	}
	envp[i] = new;
	return (envp);
}

/// @brief own implementation of setenv from stdlib.c
/// Checks if *str is valid then searches for the *v variable in the envp array.
/// If the variable is found, it changes the value of the variable.
/// If the variable is not found, it adds the variable to the array.
/// the first_time bool is used to handle the leak of the envp array.
/// @param str the name of the environment variable to set.
/// @param v value of the environment variable to set.
/// @param envp the enviroment array.
/// @param first_time if true, the function will not free the old envp.
/// @return the new envp array or NULL if an error occurred.
char	**ft_arr_setenv(const char *str, const char *v
			, char **envp, bool first_time)
{
	size_t	i;
	char	*variable;
	char	*new;

	if (!str || !v || ft_strchr(str, '=') != NULL || *str == '\0')
		return (NULL);
	variable = serch_variable(str, envp, &i);
	if (variable && !first_time)
		free(envp[i]);
	new = create_variable(str, v);
	if (!new)
		return (NULL);
	envp = update_envp(envp, new, i, first_time);
	return (envp);
}
