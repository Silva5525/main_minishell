/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:06:40 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/06 20:08:46 by wdegraf          ###   ########.fr       */
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
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	len = strlen(str);
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

/// @brief Strips quotes from the concantenated string, prints the string
/// and frees the memory.
/// @param joined_str concantenated string to print.
/// @param hold temporary pointer to the processed string without quotes.
/// @param minus_nl flag to print a newline or not.
int	echo_helper(char *joined_str, char *hold, bool minus_nl)
{
	if (joined_str && joined_str[strlen(joined_str) - 1] == ' ')
		joined_str[strlen(joined_str) - 1] = '\0';
	if (!joined_str)
		return (write(2, "Error, ft_strjoin_multiple in b_echo\n", 38),
			EXIT_FAILURE);
	hold = unclosed_quotes(joined_str);
	if (!hold)
		return (write(2, "Error, unclosed_quotes in echo_helper\n", 38),
			EXIT_FAILURE);
	hold = strip_quotes(joined_str);
	if (!hold)
		return (write(2, "Error, strip_quotes in echo_helper\n", 35),
			EXIT_FAILURE);
	printf("%s", hold);
	if (!minus_nl)
		printf("\n");
	if (joined_str)
		free(joined_str);
	if (hold)
		free(hold);
	return (EXIT_SUCCESS);
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
