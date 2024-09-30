/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 20:06:57 by wdegraf           #+#    #+#             */
/*   Updated: 2024/09/26 13:50:38 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief handles redirections to the tokens
/// @param read the string to be tokenized
/// @param arr holds all data of the minishell
void	handl_redir_to_ken(const char **read, t_arr *arr)
{
	char	buf[2];
	char	**arg;

	if (**read == '>' && *(*read + 1) == '>')
	{
		buf[0] = 'A';
		buf[1] = '\0';
		*read += 2;
	}
	else if (**read == '<' && *(*read + 1) == '<')
	{
		buf[0] = 'H';
		buf[1] = '\0';
		*read += 2;
	}
	else
	{
		buf[0] = **read;
		buf[1] = '\0';
		(*read)++;
	}
	arg = ft_split(buf, ' ');
	catch_token(arr, list_token(arg, buf[0]));
}

/// @brief handles regular tokens ($?, words, commands, arguments)
/// @param read the string to be tokenized
/// @param arr holds all data of the minishell
void	handl_to_ken(const char **read, t_arr *arr)
{
	char	buf[1024];
	char	**arg;
	size_t	i;

	i = 0;
	while (**read && !ft_isspace(**read) && !ft_strchr(">|<", **read))
		buf[i++] = *(*read)++;
	buf[i] = '\0';
	arg = split_str_to_arg(buf);
	if (ft_strstr(arg[0], "$?"))
	{
		arg[0] = doller_question(arg[0], arr->stat);
		if (!arg[0])
			error_free_exit(arr, "Error, doller_question in handl_to_ken\n");
	}
	catch_token(arr, list_token(arg, 'w'));
}

/// @brief the Tokenizer for the minishell. 
/// It tokenizes the input string and stores the tokens in the t_arr struct.
/// @param read the string to be tokenized
/// @param arr the struct that holds all data of the minishell
void	to_ken_producer(const char *read, t_arr *arr)
{
	while (*read)
	{
		if (ft_isspace(*read))
		{
			read++;
			continue ;
		}
		if (ft_strchr(">|<", *read))
		{
			handl_redir_to_ken(&read, arr);
			continue ;
		}
		handl_to_ken(&read, arr);
	}
	if (arr->size < arr->max_size)
		arr->ken[arr->size] = NULL;
}
