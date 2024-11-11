# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wdegraf <wdegraf@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 15:45:34 by wdegraf           #+#    #+#              #
#    Updated: 2024/11/11 10:31:21 by wdegraf          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell
CC		:= cc
CFLAGS	:= -Wall -Werror -Wextra -g
#-g -pthread -fsanitize=address
#-Wunreachable-code
#-g -fsanitize=address -Ofast 
# valgrind --leak-check=full ./meinProgramm

LIBFT	:= ./libft

HEADERS	:= -I ./include -I $(LIBFT)

LIBS	:= -L$(LIBFT) -lft -lreadline

SRCS := main.c lexer.c expand.c piping.c redir.c \
		built/b_export.c built/b_cd.c built/b_echo.c \
		built/b_pwd.c built/a_builtins.c built/b_exit.c built/b_env.c \
		built/b_unset.c open_quotes.c mini_helper.c \
		free_tokens.c built/ft_arr_setenv.c manage_token.c \
		tokenize.c execute.c mini_helper2.c \
		error.c lexer_2.c lexer_3.c lexer_4.c 
OBJS	:= ${SRCS:.c=.o}

all: libft $(NAME)

libft:
	@make -C $(LIBFT)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(CFLAGS) $(LIBS) $(HEADERS) -o $(NAME) 
	
clean:
	@rm -rf $(OBJS)
	@make -C $(LIBFT) clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIBFT) fclean

re: fclean all

t: $(NAME)
	./$(NAME) 

ret: re t

.PHONY: all, clean, fclean, re, libmlx, libft
