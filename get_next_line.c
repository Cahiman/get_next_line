/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: basianno <basianno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 07:47:57 by basianno          #+#    #+#             */
/*   Updated: 2026/11/12 17:23:46 by basianno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_free(char *buffer, char **stash)
{
	free(buffer);
	free(*stash);
	*stash = NULL;
	return (NULL);
}

static char	*stash_extraction(char *stash)
{
	char	*before;
	int		len;

	len = 0;
	if (!stash || stash[0] == '\0')
		return (NULL);
	while (stash[len] && stash[len] != '\n')
		len++;
	before = malloc(sizeof(char) * (len + 2));
	if (!before)
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
	{
		before[len] = stash[len];
		len++;
	}
	if (stash[len] == '\n')
		before[len++] = '\n';
	before[len] = '\0';
	return (before);
}

static char	*stash_update(char *stash)
{
	char	*after;
	int		i;
	int		len;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] != '\n')
		return (free(stash), NULL);
	i = len + 1;
	len = ft_strlen(stash);
	after = malloc(sizeof(char) * (len + 1));
	if (!after)
		return (free(stash), NULL);
	len = 0;
	while (stash[i])
	{
		after[len] = stash[i];
		len++;
		i++;
	}
	after[len] = '\0';
	free(stash);
	return (after);
}

static char	*check_verif(int fd, char **stash)
{
	char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!*stash)
		*stash = ft_strdup("");
	if (!buffer || !*stash)
	{
		free (buffer);
		return (NULL);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	ssize_t		readed;
	char		*buffer;
	char		*temp;

	readed = 1;
	buffer = check_verif(fd, &stash);
	if (!buffer)
		return (NULL);
	while (!ft_strchr(stash, '\n') && readed > 0)
	{
		readed = read(fd, buffer, BUFFER_SIZE);
		if (readed == -1)
			return (ft_free(buffer, &stash));
		buffer[readed] = '\0';
		temp = ft_strjoin(stash, buffer);
		if (!temp)
			return (ft_free(buffer, &stash));
		free (stash);
		stash = temp;
	}
	free (buffer);
	temp = stash_extraction(stash);
	stash = stash_update(stash);
	return (temp);
}

// #include <stdio.h>
// #include <fcntl.h>

// int	main(int ac, char **av)
// {
// 	int		res;
// 	char	*line;

// 	if (ac == 2)
// 	{
// 		res = open(av[1], O_RDONLY);
// 		if (res == -1)
// 			return (printf("Erreur de lecture du fichier.\n"), 0);
// 		line = get_next_line(res);
// 		while (line)
// 		{
// 			printf("%s", line);
// 			free(line);
// 			line = get_next_line(res);
// 		}
// 		close(res);
// 	}
// 	else
// 		printf("Nombre d'argument invalide.\n");
// 	return (0);
// }
