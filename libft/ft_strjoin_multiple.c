/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_multiple.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:21:24 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/06 18:20:51 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief joins multiple strings together till its not called anymore.
/// @param hold the string that is already joined. NULL if the first string.
/// @param next the string that should be joined to hold.
/// @return the new joined string.
char	*ft_strjoin_multiple(char *hold, char *next)
{
	char	*new_hold;
	size_t	i;

	if (!next)
		return (hold);
	if (!hold)
		return (ft_strdup(next));
	i = ft_strlen(hold) + ft_strlen(next);
	new_hold = malloc(i + 1);
	if (!new_hold)
		return (NULL);
	ft_strcpy(new_hold, hold);
	ft_strcat(new_hold, next);
	free(hold);
	return (new_hold);
}

/// call the funktion like this but dont forget to free the return value.
// while (i < count)
// {
// 	hold = ft_strjoin_multiple(hold, str);
// 	i++;
// 	if (!hold)
// 		return (NULL);
// }
