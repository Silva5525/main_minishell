/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:26:13 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/25 18:48:38 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief writes an error message to the standard error output
/// errno is a global variable that is set by system calls and some library
///functions in the event of an error to indicate what went wrong.
/// I use #include <errno.h> to get the error number
/// @param ENOENT No such file or directory (POSIX.1) in errno.h
/// @param EACCES Permission denied (POSIX.1) in errno.
void	command_not_found(t_arr *arr)
{
	free_tokens(arr);
	if (errno == ENOENT)
		write(2, "Error, command not found\n", 25);
	else if (errno == EACCES)
		write(2, "Error, permission denied\n", 26);
	else
		write(2, "Error, in ex_order in builtins\n", 31);
	exit(EXIT_FAILURE);
}

/// @brief is the handler for the built-in commands.
/// uses the struct t_b to compare the command with the built-in commands
/// if the command is a built-in command, the corresponding function is called
/// if the command is not a built-in command, the command is executed.
/// @param arr holds all data of the minishell
int	builtin(t_arr *arr)
{
	size_t	i;
	t_b		built[8];

	built[0] = (t_b){"echo", b_echo};
	built[1] = (t_b){"cd", b_cd};
	built[2] = (t_b){"pwd", b_pwd};
	built[3] = (t_b){"export", b_export};
	built[4] = (t_b){"unset", b_unset};
	built[5] = (t_b){"env", b_env};
	built[6] = (t_b){"exit", b_exit};
	built[7] = (t_b){NULL, NULL};
	i = 0;
	if (!arr->ken || !arr->ken[0] || !arr->ken[0]->str[0])
		return (write(2, "Error, no command found\n", 24), EXIT_FAILURE);
	while (built[i].name)
	{
		if (ft_strcmp(arr->ken[0]->str[0], built[i].name) == 0)
			return (built[i].fun(arr), EXIT_SUCCESS);
		i++;
	}
	ex_order(arr);
	return (EXIT_SUCCESS);
}
