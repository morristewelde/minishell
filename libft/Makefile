# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/07 19:03:15 by sruzzi            #+#    #+#              #
#    Updated: 2023/11/08 09:31:06 by sruzzi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a


SRC = ft_bzero.c ft_calloc.c ft_memchr.c ft_memset.c ft_memcpy.c ft_memmove.c ft_memcmp.c ft_atoi.c \
	ft_strlen.c ft_strchr.c ft_strrchr.c ft_strdup.c ft_strncmp.c ft_strlcpy.c ft_strlcat.c ft_lstnew.c ft_strncpy.c ft_strcpy.c ft_strndup.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_strcmp.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_strnstr.c ft_snprintf.c\
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c\
	ft_printf/ft_printf.c ft_printf/ft_printf_utilsB.c ft_printf/ft_printf_utilsC.c \
	get_next_line/get_next_line.c get_next_line/get_next_line_utils.c


OBJS =		$(SRC:.c=.o)

CC = 		gcc

CFLAGS =	-Wall -Wextra -Werror -I../include/

RM = 		rm -f

AR =		ar crs

all:		$(NAME)

$(NAME): 	$(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all

.PHONY:	all clean fclean re
