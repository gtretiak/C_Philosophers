/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:11:50 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 18:54:47 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_others(t_philo *philo)
{
	long	check;

	while (1)
	{
		check = get_long(&philo->table->lock, &philo->table->all_ready);
		if (check == 1)
			break ;
		else if (check == -2)
			return (1);
	}
	return (0);
}

int	all_running(t_common_data *table)
{
	int		ret;
	long	philo_nbr;
	long	n_threads;

	philo_nbr = get_long(&table->lock, &table->n_philos);
	if (philo_nbr == -2)
		return (-1);
	n_threads = get_long(&table->lock, &table->running_threads);
	if (n_threads == -2)
		return (-1);
	if (n_threads == philo_nbr)
		ret = 1;
	else
		ret = 0;
	printf("num of threads running: %ld\nphilo_nbr: %ld\n", n_threads, philo_nbr);
	fflush(stdout);
	return (ret);
}

long	get_time(t_time time)
{
	struct timeval	ts;

	if (gettimeofday(&ts, NULL))
	{
		printf("Error: gettimeofday failed.\n");
		return(-666);
	}
	if (time == MICRO)
		return (ts.tv_sec * 1e6 + ts.tv_usec);
	else if (time == MILI)
		return (ts.tv_sec * 1e3 + ts.tv_usec / 1e3);
	return (42);
}

int	precise_usleep(long duration, t_common_data *table)
{
	long	start;
	long	elapsed;
	long	remainder;
	long	dinner;

	start = get_time(MICRO);
	if (start < 0)
		return (1);
	while (get_time(MICRO) - start < duration)
	{
		dinner = get_long(&table->lock, &table->dinner_is_over);
		if (dinner == -2)
			return (1);
		if (dinner == 1)
			break ;
		elapsed = get_time(MICRO) - start;
		if (elapsed < 0)
			return (1);
		remainder = duration - elapsed;
		if (remainder > 1e3)
			usleep(remainder / 2);
		else
		{
			while (get_time(MICRO) - start < duration)
				;
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

	dinner = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	start = get_long(&philo->table->lock, &philo->table->t_start);
	pos = get_long(&philo->philo_lock, &philo->position);
	if (dinner == -2 || start == -2 || pos == -2)
		return (1);
	if (dinner == 0)
	{
		time = get_time(MILI) - start;
		if (time < 0)
			return (1);
		if (mutex_handler(&philo->table->print_lock, LOCK))
			return (1);
		printf("%ld %ld %s", time, pos, msg);
		if (mutex_handler(&philo->table->print_lock, UNLOCK))
			return (1);
	}
	return (0);
}
