/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:38 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:56:40 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	printing_status2(t_philo *philo, long pos, char *msg, long start)
{
	long	time;
	long	dinner;

	time = get_time(MS, philo->table) - start; // we calculate timestamp elapsed since the simulation´s start
	if (time < 0)
		return (1);
	if (handle_mtx(&philo->table->print_lock, LOCK, philo->table))
		return (1);
	dinner = get_long(&philo->table->lock,
			&philo->table->dinner_is_over, philo->table);
	if (dinner == -2)
	{
		handle_mtx(&philo->table->print_lock, UNLOCK, philo->table);
		return (1);
	}
	else if (dinner == 1 || (dinner == 2))
	{
		handle_mtx(&philo->table->print_lock, UNLOCK, philo->table);
		return (0);
	}
	printf("%ld %ld %s", time, pos, msg); // if dinner is not finished we print actual state of a philo
	if (handle_mtx(&philo->table->print_lock, UNLOCK, philo->table))
		return (1);
	return (0);
}

int	printing_status(t_philo *philo, char *msg)
{
	long	dinner;
	long	start;
	long	pos;

	dinner = get_long(&philo->table->lock,
			&philo->table->dinner_is_over, philo->table);
	start = get_long(&philo->table->lock, &philo->table->t_start, philo->table);
	pos = get_long(&philo->lock, &philo->pos, philo->table);
	if (dinner == -2 || start == -2 || pos == -2)
		return (1);
	if (dinner != 0) // if dinner is over we don´t need to print anymore
		return (0);
	return (printing_status2(philo, pos, msg, start));
}
