# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtuomi <jtuomi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/14 13:14:15 by jtuomi            #+#    #+#              #
#    Updated: 2024/12/04 15:01:31 by jtuomi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

AR = ar rcs
CC = cc
INCLUDE_DIRS = libft
INCLUDE = Makefile
CFLAGS = -Wall -Wextra -Werror
SRC = pipex.c
SRCS =	ft_atoi.c \
	ft_ldiv.c \
	ft_ltostr.c \
	ft_strlen.c \
	ft_printf.c \
	specifiers.c \
	more_specifiers.c
OBJ := $(SRC:%.c=%.o)
OBJS := $(addprefix libft/, :%.c=%.o)
OBJ_ALL := $(OBJ) $(OBJS)
MAKE = make -C
NAME = libftprintf.a

all: $(NAME)
$(NAME) : $(OBJ) libft.a
	$(AR) $(NAME) $(OBJ_ALL)
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
