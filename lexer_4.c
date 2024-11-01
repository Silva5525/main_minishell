/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:42:16 by wdegraf           #+#    #+#             */
/*   Updated: 2024/11/01 15:25:46 by wdegraf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief handles the pipe redirection. Sets the file descriptor first for the
/// first segment where the input is STDIN_FILENO. For the next segment the
/// input file descriptor is the output file descriptor of the previous segment.
/// For easier handling in do_fork and ex_order later.
/// @param segments the array of segments.
/// @param seg_count index of the current segment.
/// @param arr the main minishell struct.
/// @return 
bool	seg_pipe(t_arr **segments, size_t *seg_count, t_arr *arr)
{
	int		p_fd[2];

	if (pipe(p_fd) < 0)
		error_free_exit(arr, "Error: Pipe failed");
	if (segments[*seg_count]->out_fd == STDOUT_FILENO)
		segments[*seg_count]->out_fd = p_fd[1];
	else
		close(p_fd[1]);
	create_new_seg(segments, arr, ++(*seg_count));
	if (segments[*seg_count]->in_fd == STDIN_FILENO)
		segments[*seg_count]->in_fd = p_fd[0];
	else
		close(p_fd[0]);
	return (true);
}

/// @brief ads a token to the segment array.
/// @param seg the actual segment.
/// @param arr the main minishell struct.
/// @param i the index of the token.
void	token_add(t_arr *seg, t_arr *arr, size_t *i)
{
	add_token_to_seg(seg, arr, *i);
	seg->size++;
	seg->ken[seg->size] = NULL;
	(*i)++;
}

/// @brief the new segment producer loop which handles unlimited segments.
/// redirects, pipes and normal tokens. saves it in the segments array.
/// which will be saved in the main minishell struct t_arr  *arr. 
/// each segment is its own version of t_arr *arr. (nested struct)
/// @param segments each segment is a t_arr struct. 
/// @param arr the main minishell struct.
/// @param seg_count the index of the current segment. (starting by 0)
void	new_producer_loop(t_arr **segments, t_arr *arr, size_t seg_count)
{
	size_t	i;
	bool	conti;

	i = 0;
	while (i < arr->size)
	{
		conti = false;
		if ((arr->ken[i]->typ == '>'))
			conti = r_out(arr, &i, segments[seg_count]);
		else if ((arr->ken[i]->typ == 'A'))
			conti = r_append(arr, &i, segments[seg_count]);
		else if ((arr->ken[i]->typ == '<'))
			conti = r_in(arr, &i, segments[seg_count]);
		else if ((arr->ken[i]->typ == 'H'))
			conti = r_heardoc(arr, &i, segments[seg_count]);
		else if ((arr->ken[i]->typ == '|'))
		{
			conti = seg_pipe(segments, &seg_count, arr);
			i++;
		}
		else if (conti == true)
			continue ;
		else
			token_add(segments[seg_count], arr, &i);
	}
}

/// @brief counts the | and redirection tokens in the ken array.
/// so we know how many segments we need to create.
/// @param ken the array of tokens.
/// @return the number of segments needed. at least 1.
int	count_seg_for_ken(t_to **ken)
{
	int		seg_count;
	int		pr;
	size_t	i;

	seg_count = 1;
	i = 0;
	while (ken[i] != NULL && ken[i]->str[0] != NULL)
	{
		pr = p_r(ken[i]->str[0]);
		if (pr > 0)
			seg_count++;
		i++;
	}
	return (seg_count);
}

/// @brief creats a new segment and stores it in the segments array.
/// @param seg the array of segments.
/// @param arr the main minishell struct.
/// @param seg_count the current segment count.
void	create_new_seg(t_arr **seg, t_arr *arr, size_t seg_count)
{
	seg[seg_count] = flexible_seg(arr);
	if (!seg[seg_count])
		error_free_exit(arr, "Error, flexible_seg in create_new_seg\n");
}
