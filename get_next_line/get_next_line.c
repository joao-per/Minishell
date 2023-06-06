/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 12:17:14 by joao-per          #+#    #+#             */
/*   Updated: 2023/02/27 12:17:14 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlenz(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	return (i);
}

int	ft_strchrz(char c, char *line)
{
	if (!line)
		return (0);
	while (*line)
		if (*line++ == c)
			return (1);
	return (0);
}

void	cleanz(char *resto)
{
	int	i;
	int	j;

	i = ft_strlenz(resto);
	j = 0;
	while (resto[i])
		resto[j++] = resto[i++];
	while (resto[j])
		resto[j++] = '\0';
}

char	*ft_strjoinz(char *line, char *buff)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc((ft_strlenz(line) + ft_strlenz(buff) + 1) * sizeof(char));
	if (!str)
		return (0);
	while (line && line[j])
		str[i++] = line[j++];
	j = 0;
	while (buff && buff[j])
	{
		str[i] = buff[j++];
		if (str[i++] == '\n')
			break ;
	}
	str[i] = '\0';
	free (line);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	resto[2];
	char		*line;

	line = NULL;
	if (fd < 0)
		return (0);
	while (resto[0] || read(fd, resto, 1) > 0)
	{
		if (ft_strchrz('\n', resto))
		{
			cleanz(resto);
			break ;
		}
		line = ft_strjoinz(line, resto);
		cleanz(resto);
	}
	return (line);
}
