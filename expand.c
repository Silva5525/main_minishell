/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:42:54 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/19 10:18:04 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief searches for the variable in the environment array
/// if the variable is found, the value of the variable is returned
/// @param envp the environment array
/// @param str string to search
/// @return variable value or NULL if the variable is not found
char	*ft_getenv_val(char **envp, char *str)
{
	size_t		i;
	size_t		j;

	if (!str || !*str)
		return (NULL);
	i = 0;
	j = ft_strlen(str);
	while (envp[i])
	{
		if ((envp[i][j] == '=') && (ft_strncmp(envp[i], str, j) == 0))
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

/// @brief Joins the current value in hold with the env value
/// if env exitst. hold string is updated with the new cocatenated
/// string.
/// @param hold pointer to the string that holds the result so far.
/// It will be updated with the concantenate result if env is found.
/// @param env environment variable value to append to hold.
/// @param out pointer to the result string after concatenation.
static void	if_env(char **hold, char *env, char **out)
{
	if (env)
	{
		*out = ft_strjoin(*hold, env);
		free(*hold);
		if (!*out)
			return ;
		*hold = *out;
	}
}

/// @brief searches pointer search till it reaches a character that is not
/// alphanumeric or an underscore which will be replaceced with a null
/// terminator to extract the variable name for the environment lookup.
/// The replaced character is saved in safe.
/// @param search pointer to current search position, get updated to next
/// non-alphanumeric character.
/// @param safe pointer to store character that was replaced by terminator.
/// so that it can be restored after the variable name is extracted.
static void	while_search(char **search, char *safe)
{
	while ((**search && ft_isalnum(**search)) || **search == '_')
		(*search)++;
	*safe = **search;
	**search = '\0';
}

/// @brief searches for $ in the string and replaces the variable with its value
/// if the variable is not found, the variable will be replaced with an empty 
/// string and the search will continue.
/// @param str string to search
/// @param hold hold the string
/// @param envp environment array
/// @param out the expanded string
/// @return hold or NULL if an error occurred
static char	*doller_search(char *str, char *hold, char **envp, char *out)
{
	char	*doller;
	char	*search;
	char	*env;
	char	safe;

	doller = ft_strchr(str, '$');
	while (doller)
	{
		*doller = '\0';
		out = ft_strjoin(hold, str);
		free(hold);
		if (!out)
			return (NULL);
		hold = out;
		search = doller + 1;
		while_search(&search, &safe);
		env = ft_getenv_val(envp, doller + 1);
		*search = safe;
		if_env(&hold, env, &out);
		str = search;
		doller = ft_strchr(str, '$');
	}
	return (hold);
}

// char	*ft_strjoin(char const *s1, char const *s2, size_t len2)
// {
// 	char	*s3;
// 	size_t	len1;
// 	size_t	len2;

// 	if (!s1 || !s2)
// 		return (NULL);
// 	len1 = ft_strlen(s1);
// 	len2 = ft_strlen(s2);
// 	printf("len2 = %li\n", len2);
// 	printf("s2 = %s\n", s2);
// 	s3 = (char *)malloc((len1 + len2 + 1) * sizeof(char));
// 	if (s3 == NULL)
// 		return (NULL);
// 	strcpy(s3, s1);
// 	strcpy(s3 + len1, s2);
// 	return (s3);
// }

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
	char	*out2;
	char	*str;
	char	*doller_quest;

	if (!read)
		return (NULL);
	str = read;
	hold = ft_strdup("");
	if (!hold)
		return (NULL);
	doller_quest = ft_strstr(str, "$?");
	if (doller_quest)
		return (free(hold), doller_question(str, arr->stat));
	out = doller_search(str, hold, envp, NULL);
	if (!out)
		return (free(hold), NULL);
	if (ft_strncmp(str, out, ft_strlen(str)) == 0)
		return (out);
	out2 = ft_strjoin(out, str);
	free(out);
	if (!out2)
		return (NULL);
	return (out2);
}

// char	*expanding_env(char *read, char **envp, t_arr *arr)
// {
// 	char	*hold;
// 	char	*out;
// 	char	*out2;
// 	char	*str;
// 	char	*doller_quest;

// 	printf("read: %s\n", read); /// debug
// 	printf("arr->stat: %d\n", arr->stat); /// debug
// 	if (!read)
// 		return (NULL);
// 	str = read;
// 	printf("str: %s\n", str); /// debug
// 	hold = ft_strdup("");
// 	if (!hold)
// 		return (NULL);
// 	doller_quest = ft_strstr(str, "$?");
// 	printf("doller_quest: %s\n", doller_quest); /// debug
// 	if (doller_quest)
// 		return (free(hold), doller_question(str, arr->stat));
// 	out = doller_search(str, hold, envp, NULL);
// 	if (!out)
// 		return (free(hold), NULL);
// 	printf("out: %s\n", out); /// debug
// 	out2 = ft_strjoin(out, str);
// 	free(out);
// 	out = NULL;
// 	if (!out2)
// 		return (NULL);
// 	printf("out2: %s\n", out2); /// debug
// 	return (out2);
// }