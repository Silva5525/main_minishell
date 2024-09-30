/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:06:40 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/26 13:57:43 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/// @brief if the command has a -n flag it will not print a newline
/// @param minus_nl the flag for the newline
/// @param i index of the command
/// @param arr the struct with all the data
/// @return a bool if the flag is set
bool	minus_newline(bool minus_nl, size_t *i, t_arr *arr)
{
	while (strncmp(arr->ken[*i]->str[0], "-n", 2) == 0)
	{
		minus_nl = true;
		(*i)++;
	}
	return (minus_nl);
}

/// @brief strips the quotes from the string
/// @param str the string to strip the quotes from
/// @return the string without the quotes
char	*strip_quotes(const char *str)
{
	size_t	len;
	char	*new_str;

	len = strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
	{
		new_str = malloc(len - 1);
		if (!new_str)
			return (NULL);
		strncpy(new_str, str + 1, len - 2);
		new_str[len - 2] = '\0';
		return (new_str);
	}
	return (strdup(str));
}

/// @brief Strips quotes from the concantenated string, prints the string
/// and frees the memory.
/// @param joined_str concantenated string to print.
/// @param hold temporary pointer to the processed string without quotes.
/// @param minus_nl flag to print a newline or not.
void	echo_helper(char *joined_str, char *hold, bool minus_nl)
{
	if (joined_str && joined_str[strlen(joined_str) - 1] == ' ')
		joined_str[strlen(joined_str) - 1] = '\0';
	hold = strip_quotes(joined_str);
	if (!hold)
	{
		write(2, "Error, strip_quotes in echo_helper\n", 35);
		return ;
	}
	if (hold)
		printf("%s", hold);
	if (minus_nl == false)
		printf("\n");
	if (joined_str)
		free(joined_str);
	if (hold)
		free(hold);
}

/// @brief if a error occurs in ft_strjoin_multiple in b_echo
/// it will print an error message, free the tokens and exit the program.
/// @param joined_str the string that should be joined
/// @param arr the struct with all the data
void	joined_str_error(char *joined_str, t_arr *arr)
{
	if (!joined_str)
		error_free_exit(arr, "Error, ft_strjoin_multiple in b_echo\n");
}

/// @brief handles the echo command. It prints the arguments to the terminal.
/// If the -n flag is set, it will not print a newline. If no arguments are
/// given, it will print a newline.
/// @param arr holds all information about the minishell
void	b_echo(t_arr *arr)
{
	bool	minus_nl;
	size_t	i;
	char	*joined_str;

	joined_str = NULL;
	minus_nl = false;
	i = 1;
	if (arr->size == 1)
	{
		printf("\n");
		return ;
	}
	minus_nl = minus_newline(minus_nl, &i, arr);
	while (i < arr->size && arr->ken[i]->str[0])
	{
		joined_str = ft_strjoin_multiple(joined_str, arr->ken[i]->str[0]);
		joined_str_error(joined_str, arr);
		if (i < arr->size - 1)
		{
			joined_str = ft_strjoin_multiple(joined_str, " ");
			joined_str_error(joined_str, arr);
		}
		i++;
	}
	echo_helper(joined_str, NULL, minus_nl);
}
