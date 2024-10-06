/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:30:02 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/06 19:38:08 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief checks the input for open quotes. If it finds ' or " it sets his
/// bool to the oposite of it self. It only returns false if both
/// quatation marks are closed.
/// @param line the line to check
/// @return if one is open it returns true, if both are closed it returns
/// false, if both are open it returns true
bool	has_open_quotes(const char *line)
{
	bool	single_q;
	bool	double_q;

	single_q = false;
	double_q = false;
	while (*line)
	{
		if (*line == '\'' && !double_q)
			single_q = !single_q;
		else if (*line == '\"' && !single_q)
			double_q = !double_q;
		line++;
	}
	return (single_q || double_q);
}

/// @brief joins two lines together
/// @param hold the first line before "> "
/// @param new_line the line after "> "
/// @return the joined line
char	*join_line(char *hold, char *new_line)
{
	char	*out;
	size_t	len;

	len = strlen(hold) + strlen(new_line) + 2;
	out = (char *)malloc(sizeof(char) * len);
	if (!out)
		return (NULL);
	strcpy(out, hold);
	strcat(out, "\n");
	strcat(out, new_line);
	return (out);
}

/// @brief if the input has open quotes it will read print "> " and read the
/// next line. It will then join the two lines and return the new line.
/// @param line the line to check
/// @return the joined line
char	*unclosed_quotes(char *line)
{
	char	*new_line;
	char	*out;
	char	*hold;

	if (!has_open_quotes(line))
		return (line);
	out = ft_strdup(line);
	if (!out)
		return (NULL);
	while (has_open_quotes(out))
	{
		new_line = readline("> ");
		if (!new_line)
			return (free(out), NULL);
		hold = out;
		out = join_line(hold, new_line);
		free(hold);
		free(new_line);
		if (!out)
			return (NULL);
	}
	return (out);
}
