/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervision.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:54:57 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 19:06:34 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_philo_dead(t_philo *philo)
{
	long	t_curr;
	long	temp;

	t_curr = get_time(MS, philo->table);
	if (t_curr < 0)
		return (-2);
	temp = get_long(&philo->lock, &philo->t_last_meal, philo->table);
	if (temp == -2)
		return (-2);
	t_curr -= temp; // we calculate how much time has passed since last meal
	temp = get_long(&philo->table->lock, &philo->table->t_die, philo->table);
	if (temp == -2)
		return (-2);
	if (temp < t_curr) // then we compare it with t_to_die
	{
		if (printing_status(philo, DIE))
			return (-2);
		if (set_long(&philo->lock, &philo->rip, 1, philo->table))
			return (-2);
		if (set_long(&philo->table->lock,
				&philo->table->dinner_is_over, 1, philo->table))
			return (-2);
		return (1);
	}
	return (0);
}

static int	update_fullness(t_philo *philo, long iter)
{
	long	temp;

	if (increase_long(&philo->table->lock,
			&philo->table->n_full, philo->table))
		return (-1); // At first, we increase the number of full philos
	temp = get_long(&philo->table->lock,
			&philo->table->n_full, philo->table);
	if (temp < 0)
		return (temp);
	if (temp >= iter) // if their number is equal to the total number of philos, we are done
	{
		if (set_long(&philo->table->lock,
				&philo->table->dinner_is_over, 2, philo->table))
			return (-1);
		return (1);
	}
	// if not, we mark the philo´s fullness as already counted
	if (set_long(&philo->lock,
			&philo->counted_full, 1, philo->table))
		return (-1);
	return (0);
}

int	is_everyone_full(t_philo *philo, long iter)
{
	long	temp;

	temp = get_long(&philo->lock,
			&philo->full, philo->table);
	if (temp < 0)
		return (temp);
	else if (temp == 1) // if the current philo is full, let´s see if they are counted
	{
		temp = get_long(&philo->lock,
				&philo->counted_full, philo->table);
		if (temp < 0)
			return (temp);
		else if (temp == 1) // if it is, we just return, there´s nothing to do
			return (0);
		return (update_fullness(philo, iter)); // if not, we proceed 
	}
	return (0);
}
