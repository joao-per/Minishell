/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 13:08:10 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/27 13:16:30 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	print_error_messages(char type_error)
{
	if (type_error == 'U')
		printf("error: unclosed quotes\n");
	if (type_error == 'S')
		printf("error: special character\n");
	return (0);
}