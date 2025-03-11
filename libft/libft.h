/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <sruzzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:49:26 by sruzzi            #+#    #+#             */
/*   Updated: 2023/11/08 11:12:21 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>
# include "ft_printf/ft_printf.h"
# include "get_next_line/get_next_line.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}			t_list;

t_list		*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del) (void*));
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
int			ft_atoi(const char *str);
int			ft_isalnum(int c);
int			ft_isalpha(int arg);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
int			ft_isspace(int c);
int			ft_lstsize(t_list *lst);
int			ft_tolower(int c);
int			ft_toupper(int c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int			ft_snprintf(char *str, size_t size, const char *format, ...);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		**ft_split(const char *src, char c);
char		*ft_strchr(char const *str, int c);
char		*ft_strdup(const char *src);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strtrim(const char *s1, const char *set);
char		*ft_strcpy(char *dst, const char *src);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strrchr(const char *s, int searched);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strncpy(char *dst, const char *src, size_t len);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void*));
void		ft_lstclear(t_list **lst, void (*del) (void*));
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t ecount, size_t esize);
void		*ft_memalloc(size_t size);
void		*ft_memchr(void *s, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memset(void *b, int c, size_t len);
void		ft_putbase_fd(int nbr, const char *base, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(const char *s, int fd);//modified (into const char)
void		ft_striter(char *s, void (*f)(char *));
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
size_t		ft_strlcat(char *dest, const char *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlen(const char *string);

#endif				
