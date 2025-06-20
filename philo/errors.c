/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:59:29 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 13:02:23 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_error(char *msg, int code)
{
	short	i;

	i = 0;
	while (msg[i])
		write(code, &msg[i++], 1);
	if (code == 2)
		exit(EXIT_FAILURE);
}
