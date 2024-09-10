/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:49:03 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/10 17:40:53 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief own implementation of strndup of the standard library
/// It duplicates the string src up to n characters
/// @param src source string
/// @param n number of characters to copy
/// @return the duplicated string
char	*ft_strndup(const char *src, size_t n)
{
	char		*dst;
	size_t		i;

	if (!src)
		return (NULL);
	i = ft_strnlen(src, n);
	dst = (char *)malloc(sizeof(char) * (i + 1));
	if (!dst)
		return (NULL);
	ft_strncpy(dst, src, i);
	dst[i] = '\0';
	return (dst);
}

// int	main(void)
// {
// 	const char *src = "Hello World!";
// 	size_t n = 5;
// 	char *dup = ft_strndup(src, n);
// 	printf("Original: %s\n", src);
// 	printf("Duplicate: %s\n", dup);
// 	free(dup);
// 	return (EXIT_SUCCESS);
// }
