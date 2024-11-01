/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_f_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:23:01 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/25 16:58:52 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/// @brief Builds a full path from a directory path and a file name by
/// concatenating the two strings with a '/' in between.
/// @param path The directory path.
/// @param name of command or filename.
/// @return Full path to the file or NULL if an error occurred.
char	*ft_build_f_path(char *path, char *name)
{
	char	*full_p;
	char	*full_n;

	if (!path || !name)
		return (NULL);
	full_p = ft_strjoin(path, "/");
	if (!full_p)
		return (NULL);
	full_n = ft_strjoin(full_p, name);
	free(full_p);
	if (!full_n)
		return (NULL);
	return (full_n);
}

// int	main(void) /// test main
// {
// 	char	*path = "/bin";
// 	char	*name = "lists";
// 	char	*full_path;

// 	full_path = ft_build_f_path(path, name);
// 	if (!full_path)
// 	{
// 		write(2, "Error, ft_build_f_path\n", 23);
// 		return (EXIT_FAILURE);
// 	}
// 	printf("Full path: %s\n", full_path);
// 	free(full_path);
// 	return (EXIT_SUCCESS);
// }
