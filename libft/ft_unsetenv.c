/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:42:50 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/20 18:27:43 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief own implement of unsetenv from the stdlib.h it
/// removes an envirement variable from the envp array.
/// Checks if *str is valid then searches for the variable in the envp array.
/// If the variable is found, it is removed from the array.
/// @param str the str of the environment variable to remove.
/// @param envp the environment in which to remove the variable.
/// @return 0 on success, -1 on error.
int	ft_unsetenv(const char *str, char **envp)
{
	int	i;
	int	len;

	if (!str || ft_strchr(str, '=') != NULL || *str == '\0')
		return (EXIT_FAILURE);
	len = ft_strlen(str);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, len) == 0 && envp[i][len] == '=')
		{
			free(envp[i]);
			while (envp[i])
			{
				envp[i] = envp[i + 1];
				i++;
			}
			return (EXIT_SUCCESS);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

// // ########################################### test main###################
// #include <stdio.h>
// #include <string.h>

// void print_env(char **envp)
// {
// 	while (*envp)
// 	{
// 		printf("%s\n", *envp);
// 		envp++;
// 	}
// }USER

// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;

// 	printf("Before unsetenv:\n");
// 	print_env(envp);
// 	ft_unsetenv("HOME", envp);
// 	ft_unsetenv("PATH", envp);
// 	ft_unsetenv("PWD", envp);
// 	ft_unsetenv("OLDPWD", envp);
// 	ft_unsetenv("USER", envp);

// 	printf("\nAfter unsetenv:\n");
// 	print_env(envp);
// 	return (EXIT_SUCCESS);
// }
