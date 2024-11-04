/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_smite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:39:08 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 19:09:31 by wdegraf          ###   ########.fr       */
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

// Erleuterung:
// 	// Eine Zeile nach oben gehen und die Zeile löschen
// 	write(1, "\033[1A", 4);		// Cursor eine Zeile nach oben
// 	write(1, "\033[2K", 4);		// Aktuelle Zeile löschen
// 	write(1, "\r", 1);			// Cursor an den Zeilenanfang setzen