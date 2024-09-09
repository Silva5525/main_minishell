/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 19:00:58 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/02 22:01:42 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

/// @brief The ft_strcat() function appends the src string to the dst string,
/// overwriting the terminating null byte ('\0') at the end of dst, and then
/// adds a terminating null byte. The strings may not overlap, and the dst
/// string must have enough space for the result.
/// @param dst The destination string to which src will be appended.
/// @param src The source string to append to dst.
/// @return A pointer to the resulting string dst.
char	*ft_strcat(char *dst, const char *src)
{
	size_t	dst_len;
	size_t	i;

	i = 0;
	dst_len = ft_strlen(dst);
	while (src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst);
}

// #include <stdio.h>
// 	int	main(void)
// {
// 	char	dst[50] = "What should i do? ";
// 	char	src[] = "Use a Towel!";
// 	ft_strcat(dst, src);
// 	printf("Formatted: %s\n", dst);
// 	return (EXIT_SUCCESS);
// }
