/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:06:40 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/13 15:22:31 by wdegraf          ###   ########.fr       */
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
	size_t	j;

	while (arr->ken[0]->str[*i] && arr->ken[0]->str[*i][0] == '-'
			&& arr->ken[0]->str[*i][1] == 'n')
	{
		j = 2;
		while (arr->ken[0]->str[*i][j] == 'n')
			j++;
		if (arr->ken[0]->str[*i][j] != '\0')
			break ;
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

void	echo_helper(char *joined_str, char *tmp, bool minus_nl)
{
	if (joined_str && joined_str[strlen(joined_str) - 1] == ' ')
		joined_str[strlen(joined_str) - 1] = '\0';
	tmp = strip_quotes(joined_str);
	if (tmp)
		printf("%s", tmp);
	if (minus_nl == false)
		printf("\n");
	if (joined_str)
		free(joined_str);
	if (tmp)
		free(tmp);
}

void	b_echo(t_arr *arr)
{
	bool	minus_nl;
	size_t	i;
	char	*joined_str;
	char	*tmp;

	joined_str = NULL;
	tmp = NULL;
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
		joined_str = ft_strjoin_multiple(joined_str, " ");
		i++;
	}
	echo_helper(joined_str, tmp, minus_nl);
}
