/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruzzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 09:20:31 by sruzzi            #+#    #+#             */
/*   Updated: 2023/12/01 09:20:35 by sruzzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

char	*get_next_line(int fd);
char	*ft_line(char *buffer);
char	*read_file(int fd, char *buf);
char	*ftt_strjoin(char const *s1, char const *s2);
char	*ftt_strchr(const char *str, int c);
void	*ftt_calloc(size_t ecount, size_t esize);
size_t	ftt_strlen(const char *string);
//char	*ft_strdup(const char *src);
char	*ft_free(char *buffer, char *stash);
char	*ft_next(char *buffer);

#endif
