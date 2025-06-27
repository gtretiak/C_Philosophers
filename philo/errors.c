/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:59:29 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/27 18:47:59 by gtretiak         ###   ########.fr       */
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

void	cleanup(t_data *table, int code, char *msg)
{
	free(table->all_philos);
	if (table->all_forks)
		free(table->all_forks);
	handle_error(msg, code);
}
