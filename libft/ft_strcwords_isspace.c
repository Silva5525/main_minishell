/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcwords_isspace.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 14:31:03 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/04 19:01:16 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief  Counts the number of words in a string
/// @param str the string to count the words in
/// @return the countet number of words till the null terminator
ssize_t	ft_strcwords_isspace(char *str)
{
	ssize_t		i;
	bool		in_word;

	i = 0;
	in_word = false;
	while (*str)
	{
		if (ft_isspace(*str))
			in_word = false;
		else if (!in_word)
		{
			in_word = true;
			i++;
		}
		str++;
	}
	return (i);
}

// int	main(void)
// {
// 	char *str = "  Hello  World or not ";
// 	printf("Number of words: %ld\n", ft_strcwords_isspace(str));
// 	return (EXIT_SUCCESS);
// }
