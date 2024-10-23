/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 13:28:50 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:23:09 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief chechs if a is an hex character.
/// @param a check char as int
/// @return true (1) or wrong (0)
int	ft_isascii(int a)
{
	if (a >= 0 && a <= 127)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

// #include <stdio.h>
// int	main (void)
// {
// 	char	a = 128;
// 	int	out = ft_isascii(a);
// 	printf("%i", out);
// 	return (EXIT_SUCCESS);
// }
