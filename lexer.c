/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 18:27:22 by wdegraf           #+#    #+#             */
/*   Updated: 2024/07/17 14:58:55 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief 
/// @param arr 
void free_tokens(t_arr *arr)
{
	size_t i;

	i = 0;
	if (!arr)
		return ;
	if (arr->direktory)
		free(arr->direktory), arr->direktory = NULL;
	while (i < arr->size)
	{
		if (arr->ken[i])
		{
			if (arr->ken[i]->str)
				free(arr->ken[i]->str), arr->ken[i]->str = NULL;
			free(arr->ken[i]), arr->ken[i] = NULL;
		}
		i++;
	}
	if (arr->ken)
		free(arr->ken), arr->ken = NULL;
	free(arr), arr = NULL;
}

void catch_token(t_arr *arr, t_to *ken)
{
	t_to **new_ken;
	if (arr->size == arr->max_size)
	{
		arr->max_size *= 2;
		new_ken = ft_realloc(arr->ken, sizeof(t_to *) * arr->max_size / 2
				, sizeof(t_to *) * arr->max_size);
		if (!new_ken)
		{
			free_tokens(arr);
			write(2, "Error, catch_token realloc\n", 27);
			exit(EXIT_FAILURE);
		}
		arr->ken = new_ken;
	}
	arr->ken[arr->size++] = ken;
}

t_arr *flexible_arr(void)
{
	t_arr	*arr = malloc(sizeof(t_arr));
	if (!arr)
		return (write(2, "Error, flexible_arr malloc\n", 27), NULL);
	arr->ken = malloc(sizeof(t_to) * 16);
	if (!arr->ken)
	{
		free_tokens(arr);
		write(2, "Error, flexible_arr malloc\n", 27);
		return (NULL);
	}
	arr->size = 0;
	arr->max_size = 16;
	arr->direktory = NULL;
	arr->envp = NULL;
	return (arr);
}

t_to	*list_token(char *val, int typ)
{
	t_to	*ken = malloc(sizeof(t_to));
	if (!ken)
		return (NULL);
	ken->str = ft_strdup(val);
	if (!ken->str)
	{
		free(ken);
		write(2, "Error, list_token malloc\n", 25);
		return (NULL);
	}
	ken->typ = typ;
	return (ken);
}

t_arr	*to_ken_producer(const char *read)
{
	t_arr	*arr;
	char	buf[1024];
	int i;

	i = 0;
	arr = flexible_arr();
	if (!arr)
		return (NULL);
	while (*read)
	{
		if (ft_isspace(*read))
		{
			read++;
			continue ;
		}
		if (ft_strchr(">|<", *read))
		{
			buf[0] = *read;
			buf[1] = '\0';
			catch_token(arr, list_token(buf, *read));
			read++;
			continue;
		}
		while (*read && !ft_isspace(*read) && !ft_strchr(">|<", *read))
			buf[i++] = *read++;
		buf[i] = '\0';
		catch_token(arr, list_token(buf, 'w'));
		i = 0;
	}
	return (arr);
}

// int main(void)
// {
// 	const char	*read = "echo Ducks are cool | grep Ducks > output.txt";
// 	t_arr		*arr = to_ken_producer(read);
// 	size_t		i;
	
// 	i = 0;
// 	while (i < arr->size)
// 	{
// 		printf("Token: %s Type: %c\n", arr->ken[i]->str, arr->ken[i]->typ);
// 		i++;
// 	}
// 	free_tokens(arr);
// 	return (0);
// }