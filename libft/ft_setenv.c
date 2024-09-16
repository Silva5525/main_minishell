/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 21:34:14 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/16 17:35:12 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief iterates through the envp array and checks if the variable is already
/// set. if the variable is set, it frees the variable and returns the index.
/// @param envp the environment array.
/// @param str the name of the environment variable to set.
/// @param len the length of the variable name.
/// @param i index of the environment array.
/// @return allways the index of the environment array.
int	set_loop(char **envp, char *str, int len, int i)
{
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, len) == 0 && envp[i][len] == '=')
		{
			if (envp[i])
				free(envp[i]);
			break ;
		}
		i++;
	}
	return (i);
}

/// @brief own implementation of setenv from stdlib.c
/// Checks if *str is valid then searches for the *v variable in the envp array.
/// If the variable is found, it changes the value of the variable.
/// If the variable is not found, it adds the variable to the array.
/// @param str the name of the environment variable to set.
/// @param v value of the environment variable to set.
/// @param envp the enviroment array.
/// @return 0 on success, -1 on error.
int	ft_setenv(const char *str, const char *v, char **envp)
{
	char	*new;
	char	*hold;
	int		i;
	int		len;

	if (!str || !v || ft_strchr(str, '=') != NULL || *str == '\0')
		return (-1);
	len = ft_strlen(str);
	i = 0;
	i = set_loop(envp, (char *)str, len, i);
	hold = ft_strjoin(str, "=");
	if (!hold)
		return (-1);
	new = ft_strjoin(hold, v);
	free(hold);
	if (!new)
		return (-1);
	envp[i] = new;
	return (EXIT_SUCCESS);
}

// /// ########################################### test main ###################

// #include <stdio.h>

// void print_env(char **envp)
// {
// 	while (*envp)
// 	{
// 		printf("%s\n", *envp);
// 		envp++;
// 	}
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;

// 	printf("Before setenv:\n");
// 	print_env(envp);
// 	ft_setenv("TEST_VAR", "test", envp);
// 	printf("After setenv:\n");
// 	print_env(envp);
// 	return (EXIT_SUCCESS);
// }