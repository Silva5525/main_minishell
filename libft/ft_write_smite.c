/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_smite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:39:08 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 19:22:25 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief deletes the last line and writes the cursor to the beginning of
/// the line.
void	ft_write_smite(void)
{
	write(1, "\033[2K", 4);
	write(1, "\r", 1);
}

// int	main(void)
// {
// 	ft_putstr_fd("Hello World", 1);
// 	ft_write_smite();
// 	return (0);
// }

// Explanation:
// // Move up one line and delete the line
// 	write(1, "\033[1A", 4);		// Cursor one line up
// 	write(1, "\033[2K", 4);		// delete aktual line
// 	write(1, "\r", 1);			// set cursor to the beginning of line