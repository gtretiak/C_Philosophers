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

	if (!preset && printing_status(philo, THINK)) // if it´s not preset, meaning not the beginning, we print the thinking state
		return (1);
	philos_nbr = get_long(&philo->table->lock,
			&philo->table->n_philos, philo->table);
	if (philos_nbr == -2)
		return (1);
	if (philos_nbr % 2 == 0) // if number of philos is even, we exit, the system is already fair
		return (0);
	t_to_eat = get_long(&philo->table->lock,
			&philo->table->t_eat, philo->table);
	t_to_sleep = get_long(&philo->table->lock,
			&philo->table->t_sleep, philo->table);
	t_think = t_to_eat * 2 - t_to_sleep; // a philo might think as much as twice t_to_eat - t_to_sleep...
	if (t_to_eat == -2 || t_to_sleep == -2)
		return (1);
	if (t_think < 0)
		t_think = 0; // ... but not less than zero
	if (sleeping(t_think * 0.5, get_time(US, philo->table), philo->table)) // let´s sleep part of the time that was just determined
		return (1);
	return (0);
}

int	think_about(t_philo *philo)
{
	if (philo->table->n_philos % 2 == 0) // if even number of philos is given
	{
		if (philo->pos % 2 == 0) // if a certain philo position is even, let´s say 2
		{
			if (sleeping(3e4, get_time(US, philo->table), philo->table)) // it first sleep, letting odd philos to grab the forks
				return (1);
		}
	}
	else // if odd number of philos is given
	{
		if (philo->pos % 2 != 0) // if a certain philo position is odd, let´s say 3
			if (thinking_phase(philo, true)) // they think
				return (1);
	}
	return (0);
}
