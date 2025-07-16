/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:04:36 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 16:05:01 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_time time, t_table *table)
{
	struct timeval	ts;

	if (gettimeofday(&ts, NULL))
	{
		write_error(TIME, table);
		return (-666);
	}
	if (time == US)
		return (ts.tv_sec * 1e6 + ts.tv_usec); // we convert seconds to microseconds
	else if (time == MS)
		return (ts.tv_sec * 1e3 + ts.tv_usec / 1e3); // we convert seconds and microseconds to miliseconds
	return (42);
}

static int	wait_micro(long end, t_table *table)
{
	long	dinner;

	while (get_time(US, table) < end)
	{
		dinner = get_long(&table->lock, &table->dinner_is_over, table);
		if (dinner < 0)
			return (1);
		if (dinner == 1 || dinner == 2)
			return (0);
	}
	return (0);
}

int	sleeping(long duration, long start, t_table *table)
{
	long	elapsed;
	long	dinner;

	if (start < 0) // this is an error
		return (1);
	while (get_time(US, table) - start < duration) // while elapsed since start time is less than given timestamp for sleeping
	{
		dinner = get_long(&table->lock, &table->dinner_is_over, table);
		elapsed = get_time(US, table) - start;
		if (dinner < 0 || elapsed < 0)
			return (1);
		if (dinner == 1)
			break ;
		if (duration - elapsed > 1e3) // if the difference more than 1ms, we sleep half-time
			usleep((duration - elapsed) / 2);
		else // if less, we wait microseconds until the end, constantly checking if dinner is over or not
			return (wait_micro(start + duration, table));
	}
	return (0);
}
