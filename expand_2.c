/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:40:43 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/12 15:07:20 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expanding_else(char *out, char *str)
{
	size_t	len;
	char	*new_out;

	len = ft_strlen(out);
	new_out = (char *)malloc(len + 2);
	if (!new_out)
		return (NULL);
	ft_strcpy(new_out, out);
	new_out[len] = *str;
	new_out[len + 1] = '\0';
	out = new_out;
	return (out);
}

char	*expanding_quotes(char *str, char *hold, char **envp, char *out)
{
	char	*new_out;
	char	*exp;

	exp = doller_search(str, hold, envp, NULL);
	if (!exp)
		return (free(hold), NULL);
	new_out = ft_strjoin(out, exp);
	out = new_out;
	if (!out)
		return (NULL);
	while (*str && (ft_isalnum(*str) || *str == '_'))
		str++;
	return (out);
}

char	*expand_loop(char *str, char *hold, char **envp, char *out)
{
	bool	singl_q;
	bool	double_q;

	singl_q = false;
	double_q = false;
	while (*str)
	{
		if (*str == '\'' && !double_q)
			singl_q = !singl_q;
		else if (*str == '\"' && !singl_q)
			double_q = !double_q;
		else if (*str == '$' && !singl_q)
		{
			out = expanding_quotes(str, hold, envp, out);
			continue ;
		}
		else
			out = expanding_else(out, str);
		str++;
		if (!out)
			return (NULL);
	}
	return (out);
}

/// @brief expands the environment variables in the read string
/// if the string contains a $ followed by a valid variable name
/// the variable will be replaced with its value.
/// @param read the string to expand
/// @param envp environment array
/// @return the expanded string out, or NULL if an error occurred
char	*expanding_env(char *read, char **envp, t_arr *arr)
{
	char	*hold;
	char	*out;
	char	*str;

	(void)arr;
	if (!read)
		return (NULL);
	str = read;
	hold = ft_strdup("");
	if (!hold)
		return (NULL);
	out = ft_strdup("");
	if (!out)
		return (free(hold), NULL);
	out = expand_loop(str, hold, envp, out);
	if (!out)
		return (free(hold), NULL);
	return (out);
}

bool	len_read(char *read)
{
	if (strlen(read) == 0)
	{
		free(read);
		return (true);
	}
	return (false);
}
