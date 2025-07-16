/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lone_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:40:20 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:40:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_alone(t_philo *philo)
{
	if (wait_others(philo)) // at least for the waiter, because the waiter should allow to run simulation
		return (1);
	if (increase_long(&philo->table->lock,
			&philo->table->running_threads, philo->table))
		return (1);
	if (set_long(&philo->lock, &philo->t_last_meal,
			get_time(MS, philo->table), philo->table))
		return (1);
	if (set_long(&philo->fork1->lock, &philo->fork1->taken, 1, philo->table))
		return (1);
	if (printing_status(philo, FORK))
		return (1);
	return (0);
}

void	*run_alone(void *arg)
{
	t_philo	*philo;
	long	temp;

	philo = (t_philo *)arg;
	if (start_alone(philo))
		return ((void *)1);
	while (1) // endless loop until the dinner is set as over by the waiter
	{
		temp = get_long(&philo->table->lock,
				&philo->table->dinner_is_over, philo->table);
		if (temp == 1)
			return ((void *)0);
		else if (temp < 0)
			return ((void *)1);
		temp = get_long(&philo->lock, &philo->rip, philo->table); // or by dead state (which is the same here, thus might be removed)
		if (temp < 0)
			return ((void *)1);
		else if (temp == 1)
			return ((void *)0);
	}
	return ((void *)0);
}
