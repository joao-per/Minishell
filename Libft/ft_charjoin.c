/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:25:08 by joao-per          #+#    #+#             */
/*   Updated: 2023/01/25 13:25:08 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_charjoin(char const *s1, char s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*result;

	i = 0;
	if (!s1)
		return (0);
	len = ft_strlen(s1) + 1;
	result = (char *)malloc(len + 1);
	if (!result)
		return (0);
	j = 0;
	while (s1[j])
		result[i++] = s1[j++];
	j = 0;
	result[i++] = s2;
	result[i] = '\0';
	return (result);
}
