/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 13:40:59 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/17 13:42:07 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief Checks if the character is an uppercase letter.
/// @param c the character to check.
/// @return true if c is an uppercase letter, false otherwise.
int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}
