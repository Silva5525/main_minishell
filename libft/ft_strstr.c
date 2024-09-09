/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 21:58:47 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/02 21:11:42 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

/// @brief searched first occurrence of needle in haystack but looks not more 
/// then len chars are searched. Chars that appear after \0 char are not searchd
/// @param haystack string to iterate
/// @param needle to find string
/// @return if needl is NULL = haystack || if needle occurs nowhere NULL ||
/// or a pointer to the first char of the first occurance of needle returned
char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*hay2;
	const char	*n2;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack)
	{
		hay2 = haystack;
		n2 = needle;
		while (*haystack == *n2
			&& *haystack != '\0' && *n2 != '\0')
		{
			haystack++;
			n2++;
		}
		if (*n2 == '\0')
			return ((char *)hay2);
		haystack = hay2 + 1;
	}
	return (NULL);
}

// int	main(void)
// {
// // // // // // 	char		*hay = "Alles meine endchen des endes";
// // // // // // 	char		*needle = "end";
// // // // // // 	printf("%s\n", strnstr(hay, needle));
// // // // // // 	const char	*show = ft_strnstr(hay, needle);
// // // // // // 	printf("show: %s\n", show);
// char haystack[30] = "aaabcabcd";
// printf("%i\n", ft_strstr(haystack, "abcd") == haystack + 6);
// printf("%s\n", ft_strstr(haystack, "abcd");
// return (EXIT_SUCCESS);
// }
