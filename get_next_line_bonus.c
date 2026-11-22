/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabaadi <yabaadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 17:02:16 by yabaadi           #+#    #+#             */
/*   Updated: 2026/11/22 10:18:39 by yabaadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*free_rest(char **rest)
{
	free(*rest);
	*rest = NULL;
	return (NULL);
}

static int	get_line(char *buffer, int fd, char **rest)
{
	ssize_t	bytes;
	ssize_t	i;

	while (1)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (-1);
		if (bytes == 0)
			return (0);
		buffer[bytes] = '\0';
		*rest = ft_strjoin(*rest, buffer);
		if (!*rest)
			return (-1);
		i = 0;
		while ((*rest)[i])
		{
			if ((*rest)[i] == '\n')
				return (1);
			i++;
		}
	}
}

static char	*extract_line(char *rest)
{
	int	i;

	i = 0;
	while (rest[i])
	{
		if (rest[i] == '\n')
			return (ft_substr(rest, 0, i + 1));
		i++;
	}
	return (ft_strdup(rest));
}

static char	*update_rest(char *rest)
{
	int		start;
	char	*tmp;

	start = 0;
	while (rest[start] && rest[start] != '\n')
		start++;
	if (!rest[start] || (rest[start] && !rest[start + 1]))
		return (free_rest(&rest));
	start++;
	tmp = ft_substr(rest, start, ft_strlen(rest + start));
	if (!tmp)
		return (free_rest(&rest));
	free(rest);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	*rest[OPEN_MAX];
	char		*buffer;
	char		*line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (get_line(buffer, fd, &rest[fd]) < 0 || !rest[fd])
	{
		free(buffer);
		return (free_rest(&rest[fd]));
	}
	line = extract_line(rest[fd]);
	if (!line)
	{
		free(buffer);
		return (free_rest(&rest[fd]));
	}
	rest[fd] = update_rest(rest[fd]);
	free(buffer);
	return (line);
}
