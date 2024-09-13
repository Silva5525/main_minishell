/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:58:51 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/13 16:48:28 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief counts the number of characters in a string up to n
/// @param str the string to count
/// @param n max number of characters to count
/// @return length of the string up to n
size_t	ft_strnlen(const char *str, size_t n)
{
	size_t	i;

	i = 0;
	while (str[i] && i < n)
		i++;
	return (i);
}
