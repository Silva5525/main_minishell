/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:36:38 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/04 12:38:04 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief checks if the string is numeric.
/// @param str the string to check.
/// @return true if the string is numeric, false if not.
bool	ft_isnumeric(const char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

// int main(void)
// {
// 	char	*str = "1234";
// 	bool	out = ft_isnumeric(str);

// 	printf("%d", out);
// 	return (EXIT_SUCCESS);
// }