/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 22:03:56 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/13 16:27:56 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief copies the string src to dst but only maximum n bytes long.
/// (including the terminating '\0' character.)
/// @param dst destination string
/// @param src sorce string
/// @param n maximum bytes to copy
/// @return pointer to the destination string dst
char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

// #include <stdio.h>

// int main(void)
// {
// 	char src[] = "Hello, World!";
// 	char dst[20];

// 	printf("src: %s\n", src);
// 	printf("Garbage dst: %s\n", dst);
// 	ft_strcpy(dst, src);
// 	printf("dst: %s\n", dst);
// 	return (EXIT_SUCCESS);
// }
