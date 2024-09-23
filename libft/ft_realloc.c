/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:48:28 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/23 13:26:41 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief ft_realloc reallocates memory for a pointer to a new size and frees
/// the old pointer. If the new size is 0 the old pointer will be freed and
/// NULL will be returned. ft_memmove so the memory may overlap without
/// undefined behavior.
/// @param ptr the old pointer
/// @param old_size the old size of the pointer
/// @param new_size the new size of the pointer
/// @return the new pointer with the old information and the new size, or NULL
/// if malloc fails or new_size is 0.
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			ft_memmove(new_ptr, ptr, old_size);
		else
			ft_memmove(new_ptr, ptr, new_size);
		free(ptr);
	}
	return (new_ptr);
}
