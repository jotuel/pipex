# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/14 13:14:15 by jtuomi            #+#    #+#              #
#    Updated: 2025/02/06 20:31:31 by jtuomi           \__/    i                #
#                                                                              #
# **************************************************************************** #

AR = ar rcs
CC = cc
INCLUDE_DIRS = libft
INCLUDE = Makefile
CFLAGS = -Wall -Wextra -Werror -g2
SRC = pipex.c \
	pipe.c \
	utils.c
OBJ := $(SRC:%.c=%.o)
MAKE = make -C
NAME = pipex

all: $(NAME)
$(NAME) : $(OBJ) libft.a
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L $(INCLUDE_DIRS) -lft
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
libft.a:
	$(MAKE) libft all supp
clean:
	$(MAKE) libft clean
	rm -f $(OBJ)
fclean: clean
	$(MAKE) libft fclean
	rm -f $(NAME)
re: fclean all

.PHONY:
	all, libft, clean, flean, re
