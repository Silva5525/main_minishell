/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:06:33 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/13 16:49:01 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief ft_strtok is a function that splits a string into tokens
/// this function is not thread safe but simple
/// @param str string to split
/// @param delim delimeter to split the string
/// @return string token
char	*ft_strtok(char *str, const char *delim)
{
	static char		*s;
	char			*start;

	if (str)
		s = str;
	if (!s || !*s)
		return (NULL);
	while (*s && ft_strchr(delim, *s))
		s++;
	if (!*s)
		return (NULL);
	start = s;
	while (*s)
	{
		if (ft_strchr(delim, *s))
		{
			*s = '\0';
			s++;
			break ;
		}
		s++;
	}
	return (start);
}

// int	main(void)
// {
// 	char *str = ft_strdup("Hello, World! This is a test string.");
// 	char *token;

// 	token = ft_strtok(str, " ,!");
// 	while (token)
// 	{
// 		printf("%s\n", token);
// 		token = ft_strtok(NULL, " ,!");
// 	}
// 	free(str);
// 	return (EXIT_SUCCESS);
// }
