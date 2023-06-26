/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 13:08:10 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/26 10:30:53 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_error_messages(char type_error, char *error)
{
	if (type_error == 'U')
		printf("error: unclosed quotes\n");
	if (type_error == 'S')
		printf("error: special character\n");
	if (type_error == 'P')
	{
		printf("bash: syntax error near unexpected token \'%s\'\n", error);
		g_check_exit[0] = 2;
	}
	if (type_error == 'F')
		printf("%s: command not found\n", error);
	return (0);
}
