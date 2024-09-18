/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 21:21:59 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/18 13:25:33 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/// @brief Allocates (with malloc(3)) and returns a new
/// string, which is the result of the concatenation
/// of ’s1’ and ’s2’
/// @param s1 The prefix (anfangs) string
/// @param s2 The suffix (end) string
/// @return The new allocated str or Null if fails
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (s3 == NULL)
		return (NULL);
	strcpy(s3, s1);
	strcpy(s3 + len1, s2);
	return (s3);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char const	s1[] = "Alle meine ";
// 	char const	s2[] = "Coockies.";
// 	char		*s3;
// 	s3 = ft_strjoin(s1, s2);
// 	if (s3 == NULL)
// 	{
// 		printf("malloc failed.\n");
// 		return (EXIT_FAILURE);
// 	}
// 	printf("%s", s3);
// 	free((void *)s3);
// 	return (EXIT_SUCCESS);
// }
