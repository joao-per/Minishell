/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:16:03 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 12:16:03 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_double_array(char **doubles)
{
	int i;

	i = 0;
	if (!doubles)
		return ;

	while (doubles[i] != NULL)
	{
		free(doubles[i]);
		i++;
	}
	free(doubles);
}

