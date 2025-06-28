/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:11:50 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/28 20:14:41 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_others(t_philo *philo)
{
	while (1)
	{
		mutex_handler(&philo->table->lock, LOCK, NULL);
		if (philo->table->all_ready == true)
			break ;
		mutex_handler(&philo->table->lock, UNLOCK, NULL);
	}
}
