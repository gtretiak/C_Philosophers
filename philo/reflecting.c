/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflecting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:02:02 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 19:02:07 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	thinking_phase(t_philo *philo, bool preset)
{
	long	t_think;
	long	t_to_eat;
	long	t_to_sleep;
	long	philos_nbr;

	if (!preset && printing_status(philo, THINK))
		return (1);
	philos_nbr = get_long(&philo->table->lock,
			&philo->table->n_philos, philo->table);
	if (philos_nbr == -2)
		return (1);
	if (philos_nbr % 2 == 0)
		return (0);
	t_to_eat = get_long(&philo->table->lock,
			&philo->table->t_eat, philo->table);
	t_to_sleep = get_long(&philo->table->lock,
			&philo->table->t_sleep, philo->table);
	t_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_eat == -2 || t_to_sleep == -2)
		return (1);
	if (t_think < 0)
		t_think = 0;
	if (sleeping(t_think * 0.5, get_time(US, philo->table), philo->table))
		return (1);
	return (0);
}

int	think_about(t_philo *philo)
{
	if (philo->table->n_philos % 2 == 0)
	{
		if (philo->pos % 2 == 0)
		{
			if (sleeping(3e4, get_time(US, philo->table), philo->table))
				return (1);
		}
	}
	else
	{
		if (philo->pos % 2 != 0)
			if (thinking_phase(philo, true))
				return (1);
	}
	return (0);
}
