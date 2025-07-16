/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:46:44 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:47:52 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	try_to_grab_forks(t_philo *philo)
{
	long	temp;

	while (1)
	{
		temp = get_long(&philo->table->lock,
				&philo->table->dinner_is_over, philo->table);
		if (temp == 1 || temp == 2)
			return (0);
		else if (temp < 0)
			return (1);
		if (set_long(&philo->fork1->lock,
				&philo->fork1->taken, 1, philo->table))
			return (1);
		temp = get_long(&philo->fork2->lock,
				&philo->fork2->taken, philo->table);
		if (temp < 0)
			return (1);
		else if (temp == 0)
			break ;
		// otherwise the fork is already taken, so rollback
		if (set_long(&philo->fork1->lock,
				&philo->fork1->taken, 0, philo->table))
			return (1);
		usleep(100 + (philo->pos % 10) * 100); // to avoid tight loop and therefore a livelock
	}
	return (0);
}

static int	do_eating(t_philo *philo)
{
	long	time;

	if (printing_status(philo, FORK)) // the philo had taken the first fork in try-function, so let´s print it
		return (1);
	if (set_long(&philo->fork2->lock, &philo->fork2->taken, 1, philo->table))
		return (1);
	if (printing_status(philo, FORK))
		return (1);
	time = get_time(MS, philo->table);
	if (time < 0
		|| set_long(&philo->lock, &philo->t_last_meal,
			get_time(MS, philo->table), philo->table))
		return (1);
	if (printing_status(philo, EAT))
		return (1);
	if (sleeping(philo->table->t_eat * 1e3,
			get_time(US, philo->table), philo->table)) // actual eating
		return (1);
	if (set_long(&philo->fork2->lock, &philo->fork2->taken, 0, philo->table)) // realising the forks
		return (1);
	if (set_long(&philo->fork1->lock, &philo->fork1->taken, 0, philo->table))
		return (1);
	return (0);
}

int	eating_phase(t_philo *philo)
{
	long	meals_nbr;
	long	eaten;

	if (try_to_grab_forks(philo)) // to avoid a deadlock and a livelock
		return (1);
	if (do_eating(philo))
		return (1);
	meals_nbr = get_long(&philo->table->lock,
			&philo->table->n_meals, philo->table);
	if (meals_nbr == -2
		|| increase_long(&philo->lock, &philo->meals_eaten, philo->table)) // we add the last meal to the total number of meals eaten by a philo
		return (1);
	eaten = get_long(&philo->lock, &philo->meals_eaten, philo->table);
	if (eaten < 0)
		return (1);
	if (meals_nbr != -1 && eaten >= meals_nbr) // if the meals_nbr flag is not -1, we compare
	{
		if (set_long(&philo->lock, &philo->full, 1, philo->table)) // this particular philo is full
			return (1);
	}
	return (0);
}
