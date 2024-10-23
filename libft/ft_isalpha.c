/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:09:52 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/23 16:23:11 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief checks if a is an alphabetical char.
/// @param a char to check representet als int
/// @return if true (1) if wront (0)
int	ft_isalpha(int a)
{
	if ((a >= 'a') && (a <= 'z'))
		return (EXIT_FAILURE);
	else if ((a >= 'A') && (a <= 'Z'))
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char	a;

// 	a = '1';
// 	int out = ft_isalpha(a);
// 	printf("%i", out);
// 	return (EXIT_SUCCESS);
// }
