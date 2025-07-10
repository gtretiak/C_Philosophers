/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:11:50 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 15:24:20 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_others(t_philo *philo)
{
	long	check;

	while (1)
	{
		check = get_long(&philo->table->lock, &philo->table->all_ready, philo->table);
		if (check == 1)
			return (0);
		else if (check == -2)
			return (1);
		usleep(100);
	}
}

int	all_running(t_table *table)
{
	long	philo_nbr;
	long	n_threads;

	philo_nbr = get_long(&table->lock, &table->n_philos, table);
	if (philo_nbr == -2)
		return (-1);
	n_threads = get_long(&table->lock, &table->running_threads, table);
	if (n_threads == -2)
		return (-1);
	if (n_threads == philo_nbr)
		return (1);
	//printf("%ld threads running, philo_nbr:%ld\n", n_threads, philo_nbr);
	return (0);
}

long	get_time(t_time time, t_table *table)
{
	struct timeval	ts;

	if (gettimeofday(&ts, NULL))
	{
		write_error(TIME, table);
		return (-666);
	}
	if (time == US)
		return (ts.tv_sec * 1e6 + ts.tv_usec);
	else if (time == MS)
		return (ts.tv_sec * 1e3 + ts.tv_usec / 1e3);
	return (42);
}

int	sleeping(long duration, long start, t_table *table)
{
	long	elapsed;
	long	dinner;

	if (start < 0)
		return (1);
	while (get_time(US, table) - start < duration)
	{
		dinner = get_long(&table->lock, &table->dinner_is_over, table);
		if (dinner == -2)
			return (1);
		if (dinner == 1)
			break ;
		elapsed = get_time(US, table) - start;
		if (elapsed < 0)
			return (1);
		if (duration - elapsed > 1e3)
			usleep((duration - elapsed) / 2);
		else
		{
			while (get_time(US, table) - start < duration)
			{
				dinner = get_long(&table->lock, &table->dinner_is_over, table);
				if (dinner == -2)
					return (1);
				if (dinner == 1)
					return (0);
				//usleep(50);
			}
		}
	}
	return (0);
}

int	printing_status(t_philo *philo, char *msg)
{
	long	time;
	long	dinner;
	long	start;
	long	pos;

	dinner = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
	start = get_long(&philo->table->lock, &philo->table->t_start, philo->table);
	pos = get_long(&philo->lock, &philo->pos, philo->table);
	if (dinner == -2 || start == -2 || pos == -2)
		return (1);
	if (dinner == 0)
	{
		time = get_time(MS, philo->table) - start;
		if (time < 0)
			return (1);
		if (handle_mtx(&philo->table->print_lock, LOCK, philo->table))
			return (1);
		dinner = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
		if (dinner == -2)
			return (1);
		else if (dinner == 1 || (dinner == 2))
			return (0);
		printf("%ld %ld %s", time, pos, msg);
		if (handle_mtx(&philo->table->print_lock, UNLOCK, philo->table))
			return (1);
	}
	return (0);
}
