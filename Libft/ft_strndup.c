/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:19:03 by joao-per          #+#    #+#             */
/*   Updated: 2023/04/05 14:19:03 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t len = ft_strlen(s);
    if (n < len)
        len = n;
    char *d = malloc(len + 1);
    if (d == NULL)
        return NULL;
    ft_memcpy(d, s, len);
    d[len] = '\0';
    return (d);
}