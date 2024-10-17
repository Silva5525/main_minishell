/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 20:06:57 by wdegraf           #+#    #+#             */
/*   Updated: 2024/10/17 22:41:34 by wdegraf          ###   ########.fr       */
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

/// @brief
/// @param seg 
void	free_seg(t_arr **seg)
{
	size_t	i;

	if (!seg)
		return ;
	i = 0;
	// printf("FREE debug seg[0]->seg_count: %zu\n", i);
	while (seg && seg[i])
	{
		if (seg[i])
		{
			free_tokens(seg[i]);
			seg[i] = NULL;
		}
		i++;
	}
	free(seg);
	seg = NULL;
}

t_arr	*flexible_seg(t_arr *arr)
{
	t_arr	*seg;

	seg = malloc(sizeof(t_arr));
	if (!seg)
		error_free_exit(arr, "Error, malloc1 in flexible_seg\n");
	seg->ken = malloc(sizeof(t_to) * arr->max_size);
	if (!seg->ken)
		error_free_exit(arr, "Error, malloc2 in flexible_seg\n");
	seg->size = 0;
	seg->max_size = arr->max_size;
	seg->direktory = arr->direktory;
	seg->envp = NULL;
	seg->hold = NULL;
	seg->in_fd = arr->in_fd;
	seg->out_fd = arr->out_fd;
	seg->stat = arr->stat;
	seg->first_time = arr->first_time;
	seg->stdin = arr->stdin;
	seg->pid = arr->pid;
	seg->seg_count = arr->seg_count;
	seg->seg = arr->seg;
	seg->arr = arr;
	return (seg);
}

void	add_token_to_seg(t_arr *seg, t_arr *arr, size_t index)
{
	t_to	*ken;

	ken = malloc(sizeof(t_to));
	if (!ken)
		error_free_exit(arr, "Error, malloc in add_token_to_seg\n");
	ken->str = malloc(sizeof(char *) * 2);
	if (!ken->str)
	{
		free(ken);
		error_free_exit(arr, "Error, malloc for ken in add_token_to_seg\n");
	}
	ken->str[0] = ft_strdup(arr->ken[index]->str[0]);
	if (!ken->str[0])
	{
		free(ken->str);
		free(ken);
		error_free_exit(arr, "Error, ft_strdup in add_token_to_seg\n");
	}
	ken->str[1] = NULL;
	ken->typ = arr->ken[index]->typ;
	catch_token(seg, ken);
	seg->size--;
	printf("do  it %zu\n", arr->size); // debug print
	printf("Copied token to seg->ken[%zu]->str[0]: %s\n", seg->size, seg->ken[seg->size]->str[0]); /// debug
}

/// @brief Checks if the current state is outside of quotes.
/// @param state the current quote state
/// (0 = outside, 1 = single quotes, 2 = double quotes)
/// @return true if outside quotes, false otherwise
bool	is_outside_quotes(int state)
{
	return (state == 0);
}

/// @brief Updates the quote state and returns whether the character
/// is inside quotes.
/// @param c the current character
/// @param state an integer representing the current state:
/// 0 means outside of quotes, 1 means inside single quotes, 2 means 
/// inside double quotes.
/// @return the updated state of whether the character is within quotes or not.
int	update_quote_state(char c, int state)
{
	if (c == '\'' && state == 0)
		return (1);
	else if (c == '\'' && state == 1)
		return (0);
	else if (c == '\"' && state == 0)
		return (2);
	else if (c == '\"' && state == 2)
		return (0);
	return (state);
}

int	p_r(const char *str)
{
	if (ft_strncmp(str, ">>", 2) == 0 || ft_strncmp(str, "<<", 2) == 0)
		return (2);
	else if (*str == '>' || *str == '<' || *str == '|')
		return (1);
	else
		return (0);
}

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
		{
			seg_count++;
		}
		i++;
	}
	return (seg_count);
}

char	*ken_after_redir(const char *read)
{
	size_t		ken_l;
	const char	*start;
	int			q;

	start = read;
	ken_l = 0;
	q = 0;
	while (*read && (!ft_isspace(*read)
			|| !is_outside_quotes(q)) && !ft_strchr(">|<", *read))
	{
		q = update_quote_state(*read, q);
		read++;
		ken_l++;
	}
	return (ft_strndup(start, ken_l));
}

void	redir_append(t_arr *seg, t_arr *arr, const char *read)
{
	int		fd;
	char	*f_name;

	read += 2;
	while (ft_isspace(*read))
		read++;
	f_name = ken_after_redir(read);
	if (!f_name)
		error_free_exit(arr, "Error, ken_after_redir in redir_append\n");
	fd = open(f_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(f_name);
	if (fd < 0)
		error_free_exit(arr, "Error, open in redir_append\n");
	if (seg->out_fd != STDOUT_FILENO)
		close(seg->out_fd);
	seg->out_fd = fd;
	while (*read && !ft_isspace(*read) && !ft_strchr(">|<", *read))
		read++;
}

/// @brief Creates a heredoc, reads input from the user until the delimiter 
/// (stopword) is encountered, and redirects the input to the pipe.
/// @param seg the segment to store the redirection
/// @param arr the minishell data structure
/// @param read the input string containing the heredoc command
void	redir_heardoc(t_arr *seg, t_arr *arr, const char *read)
{
	char	*delim;
	char	*str;
	int		fd[2];

	read += 2;
	while (ft_isspace(*read))
		read++;
	delim = ken_after_redir(read);
	if (!delim)
		error_free_exit(arr, "Error, ken_after_redir in redir_heardoc\n");
	if (pipe(fd) < 0)
	{
		free(delim);
		error_free_exit(arr, "Error, pipe in redir_heardoc\n");
	}
	while (1)
	{
		str = readline("> ");
		if (!str)
			break ;
		if (ft_strcmp(str, delim) == 0)
		{
			free(str);
			break ;
		}
		write(fd[1], str, ft_strlen(str));
		write(fd[1], "\n", 1);
		free(str);
	}
	free(delim);
	close(fd[1]);
	if (seg->in_fd != STDIN_FILENO)
		close(seg->in_fd);
	seg->in_fd = fd[0];
}

/// @brief Handles output redirection `>`. Redirects the output to the
/// specified file. Opens the file in overwrite mode and redirects stdout
/// to the file.
/// @param seg the current segment (t_arr) to store redirection details
/// @param arr the main shell structure
/// @param read the input string containing the redirection
void	redir_out(t_arr *seg, t_arr *arr, const char *read)
{
	int		fd;
	char	*f_name;

	f_name = ken_after_redir(read);
	if (!f_name)
		error_free_exit(arr, "Error, ken_after_redir in redir_out\n");
	fd = open(f_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(f_name);
	if (fd < 0)
		error_free_exit(arr, "Error, open in redir_out\n");
	if (seg->out_fd != STDOUT_FILENO)
		close(seg->out_fd);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		error_free_exit(arr, "Error, dup2 in redir_out\n");
	}
	seg->out_fd = fd;
	while (*read && !ft_isspace(*read) && !ft_strchr(">|<", *read))
		read++;
}

/// @brief Handles input redirection (`<`), opens the specified file for reading
/// and redirects the input to the file.
/// @param seg the segment to store the redirection
/// @param arr the minishell data structure
/// @param read the input string containing the redirection command
void	redir_in(t_arr *seg, t_arr *arr, const char *read)
{
	int		fd;
	char	*f_name;

	read++;
	while (ft_isspace(*read))
		read++;
	f_name = ken_after_redir(read);
	if (!f_name)
		error_free_exit(arr, "Error, ken_after_redir in redir_in\n");
	fd = open(f_name, O_RDONLY);
	free(f_name);
	if (fd < 0)
		error_free_exit(arr, "Error, open in redir_in\n");
	if (seg->in_fd != STDIN_FILENO)
		close(seg->in_fd);
	seg->in_fd = fd;
}

void	create_new_seg(t_arr **seg, t_arr *arr, size_t *seg_count)
{
	if (*seg_count >= 1)
		(*seg_count)++;
	seg[*seg_count] = flexible_seg(arr);
	if (!seg[*seg_count])
		error_free_exit(arr, "Error, flexible_seg in create_new_seg\n");
}

void	new_to_ken_producer(t_arr *arr)
{
	t_arr	**segments;
	size_t	seg_count;
	size_t	i;
	size_t	j;

	seg_count = 0;
	i = 0;
	j = count_seg_for_ken(arr->ken);
	segments = malloc(sizeof(t_arr *) * (j + 1));
	if (!segments)
		error_free_exit(arr, "Error, malloc in new_to_ken_producer\n");
	create_new_seg(segments, arr, &seg_count);
	while (i < arr->size)
	{
		if (ft_strcmp(arr->ken[i]->str[0], ">") == 0)
		{
			redir_out(segments[seg_count], arr, arr->ken[++i]->str[0]);
			i++;
			continue ;
		}
		else if (ft_strcmp(arr->ken[i]->str[0], ">>") == 0)
		{
			redir_append(segments[seg_count], arr, arr->ken[++i]->str[0]);
			i++;
			continue ;
		}
		else if (ft_strcmp(arr->ken[i]->str[0], "<") == 0)
		{
			redir_in(segments[seg_count], arr, arr->ken[++i]->str[0]);
			i++;
			continue ;
		}
		else if (ft_strcmp(arr->ken[i]->str[0], "<<") == 0)
		{
			redir_heardoc(segments[seg_count], arr, arr->ken[++i]->str[0]);
			i++;
			continue ;
		}
		else if (ft_strcmp(arr->ken[i]->str[0], "|") == 0)
		{
			create_new_seg(segments, arr, &seg_count);
			arr->seg[seg_count] = segments[seg_count];
			i++;
			continue ;
		}
		else
		{
			add_token_to_seg(segments[seg_count], arr, i);
			// printf(" 1show segment %zu\n", seg_count); //debug
		}
		i++;
		segments[seg_count]->size++;
	}
	// print_ken_strings(arr); // debug
	// printf(" 0show segment %zu\n", i); // debug
	print_ken_strings(segments[seg_count]); // debug
	printf(" 2show segment %zu\n", seg_count); // debug
	segments[seg_count + 1] = NULL;
	arr->seg = segments;
	arr->seg_count = seg_count;
}
\
// char **split_with_quotes(const char *str, t_arr *arr, size_t i, size_t j)
// {
//     char    **tokens;
//     size_t  token_start;
//     int     q;

//     q = 0;
//     token_start = 0;
//     tokens = (char **)malloc(sizeof(char *) * arr->max_size);
//     if (!tokens)
//         error_free_exit(arr, "Error, malloc in split_with_quotes\n");

//     printf("split_with_quotes: processing string: %s\n", str);  // Debug print

//     while (str[i])
//     {
//         q = update_quote_state(str[i], q);
//         if (ft_isspace(str[i]) && is_outside_quotes(q))
//         {
//             if (i > token_start)
//             {
//                 tokens[j++] = ft_strndup(str + token_start, i - token_start);
//                 printf("Token found: %s\n", tokens[j-1]);  // Debug print
//                 if (j >= arr->max_size)
//                 {
//                     arr->max_size *= 2;
//                     tokens = realloc(tokens, sizeof(char *) * arr->max_size);
//                     if (!tokens)
//                         error_free_exit(arr, "Error, realloc in split_with_quotes\n");
//                 }
//             }
//             token_start = i + 1;
//         }
//         i++;
//     }
//     if (i > token_start)
//     {
//         tokens[j++] = ft_strndup(str + token_start, i - token_start);
//         printf("Final token: %s\n", tokens[j-1]);  // Debug print
//     }
//     tokens[j] = NULL;
//     return (tokens);
// }

void print_ken_strings(t_arr *arr)
{
    size_t i, j;

    // Loop through all the tokens in arr->ken
    for (i = 0; i < arr->size; i++)
    {
		if (arr == NULL)
			{ printf("arr is NULL\n"); return ;}
		if (arr->ken == NULL)
			{ printf("arr->ken is NULL\n"); return ;}
        // Check if the current token (ken[i]) is not NULL
        if (arr->ken[0] != NULL && arr->ken[i]->str[0] != NULL)
        {
            printf("Token %zu:\n", i);
            // Loop through the strings inside the current token (ken[i]->str)
            j = 0;
            while (arr->ken[i]->str[j] != NULL)
            {
                printf("printi print  ken[%zu]->str[%zu]: %s\n", i, j, arr->ken[i]->str[j]);
                j++;
            }
        }
        else
        {
            printf("Token %zu is NULL or has no strings!\n", i);
        }
    }
}