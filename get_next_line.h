/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 11:32:27 by marvin            #+#    #+#             */
/*   Updated: 2026/11/08 11:32:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H
# define BUFFER_SIZE 42
# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	ft_strlen(const char *s)
char	*strchr(const char *s, int c);

#endif