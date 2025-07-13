/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:11:50 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/13 12:45:49 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	wait_others(t_philo *philo)
{
	long	check;

	while (1)
	{
		check = get_long(&philo->table->lock,
				&philo->table->all_ready, philo->table);
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
	return (0);
}

int	ft_strncmp(char *s1, char *s2, int n)
{
	while (n)
	{
		if (*s1 != *s2)
			return (1);
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
		n--;
	}
	return (0);
}
