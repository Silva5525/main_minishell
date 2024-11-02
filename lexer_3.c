/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:01:12 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/02 14:57:50 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief starts redirection for redir_aout in the segments.
/// @param arr the main minishell struct
/// @param i the index of the token
/// @param seg the segment we are currently working on
/// @return true for the continue of the loop.
bool	r_out(t_arr *arr, size_t *i, t_arr *seg)
{
	if (arr->ken[++(*i)] == NULL || arr->ken[*i]->str[0] == NULL)
		error_free_exit(arr, "Error: No delimiter specified after '<<'");
	else
	{
		redir_out(seg, arr, arr->ken[*i]->str[0]);
		(*i)++;
	}
	return (true);
}

/// @brief starts redirection for redir_append in the segments.
/// @param arr the main minishell struct
/// @param i the index of the token
/// @param seg the segment we are currently working on
/// @return true for the continue of the loop.
bool	r_append(t_arr *arr, size_t *i, t_arr *seg)
{
	if (arr->ken[++(*i)] == NULL || arr->ken[*i]->str[0] == NULL)
		error_free_exit(arr, "Error: No delimiter specified after '<<'");
	else
	{
		redir_append(seg, arr, arr->ken[*i]->str[0]);
		(*i)++;
	}
	return (true);
}

/// @brief starts redirection for redir_in in the segments.
/// @param arr the main minishell struct
/// @param i the index of the token
/// @param seg the segment we are currently working on
/// @return true for the continue of the loop.
bool	r_in(t_arr *arr, size_t *i, t_arr *seg)
{
	if (arr->ken[++(*i)] == NULL || arr->ken[*i]->str[0] == NULL)
		error_free_exit(arr, "Error: No delimiter specified after '<<'");
	else
	{
		redir_in(seg, arr, arr->ken[*i]->str[0]);
		(*i)++;
	}
	return (true);
}

/// @brief starts redirection for the heredoc in the segments.
/// @param arr the main minishell struct
/// @param i the index of the token
/// @param seg the segment we are currently working on
/// @return true for the continue of the loop.
bool	r_heardoc(t_arr *arr, size_t *i, t_arr *seg)
{
	if (arr->ken[++(*i)] == NULL || arr->ken[*i]->str[0] == NULL)
		error_free_exit(arr, "Error: No delimiter specified after '<<'");
	else
	{
		redir_heardoc(seg, arr, arr->ken[*i]->str[0]);
		(*i)++;
	}
	return (true);
}

/// @brief a producer that creates new segments for the minishell.
/// @param arr it holds all the data of the minishell.
/// we store the new segments in the arr->seg array.
void	new_to_ken_producer(t_arr *arr)
{
	t_arr	**segments;
	size_t	j;
	size_t	seg_count;

	seg_count = 0;
	j = count_seg_for_ken(arr->ken);
	segments = (t_arr **)malloc(sizeof(t_arr *) * (j + 1));
	if (!segments)
		error_free_exit(arr, "Error, malloc in new_to_ken_producer\n");
	create_new_seg(segments, arr, seg_count);
	new_producer_loop(segments, arr, seg_count);
	while (seg_count < j)
	{
		segments[seg_count]->seg_count = j;
		seg_count++;
	}
	seg_count--;
	segments[seg_count + 1] = NULL;
	arr->seg_count = seg_count;
	arr->seg = segments;
}
