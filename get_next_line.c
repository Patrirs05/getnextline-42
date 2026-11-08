/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line..c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 11:32:20 by marvin            #+#    #+#             */
/*   Updated: 2026/11/08 11:32:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*temp_buffer;
	char		*readed_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
    	stash = ft_strdup("");
	readed_bytes = 1;
	temp_buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!temp_buffer)
		return (NULL);
	while (readed_bytes > 0 && !strchr(stash, '\n'))
	{
		readed_bytes = read(fd, temp_buffer, BUFFER_SIZE);
		if (readed_bytes == -1)
		{
			free(temp_buffer);
			return (NULL);
		}
		temp_buffer[readed_bytes] = '\0';
		stash = ft_strjoin(stash, temp_buffer);
	}
	return (extract_line(&stash, temp_buffer));
}
