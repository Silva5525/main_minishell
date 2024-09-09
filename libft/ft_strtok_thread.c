/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_thread.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 15:14:59 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/04 19:20:22 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief ft_strtok_thread is a function that splits a string into tokens
/// this function is thread safe 
/// @param str string to split
/// @param delim delimeter to split the string
/// @param saveptr save the position of the next token
/// @return the next token
char	*ft_strtok_thread(char *str, const char *delim, char **saveptr)
{
	char	*start;

	if (str)
		*saveptr = str;
	if (!*saveptr || !**saveptr)
		return (NULL);
	while (**saveptr && ft_strchr(delim, **saveptr))
		(*saveptr)++;
	if (!**saveptr)
		return (NULL);
	start = *saveptr;
	while (**saveptr)
	{
		if (ft_strchr(delim, **saveptr))
		{
			**saveptr = '\0';
			(*saveptr)++;
			break ;
		}
		(*saveptr)++;
	}
	return (start);
}

// int	main(void)
// {
// 	char *str = ft_strdup("Hello, World! This is a test string.");
// 	char *saveptr;
// 	char *token;

// 	token = ft_strtok_thread(str, " ,!", &saveptr);
// 	while (token)
// 	{
// 		printf("%s\n", token);
// 		token = ft_strtok_thread(NULL, " ,!", &saveptr);
// 	}
// 	free(str);
// 	return (EXIT_SUCCESS);
// }
