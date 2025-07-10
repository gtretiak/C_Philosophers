/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:55:52 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	before_start(t_philo *philo)
{
	if (wait_others(philo))
		return (1);
	if (increase_long(&philo->table->lock,
			&philo->table->running_threads, philo->table))
		return (1);
	if (set_long(&philo->lock, &philo->t_last_meal,
			get_time(MS, philo->table), philo->table))
		return (1);
	if (think_about(philo))
		return (1);
	return (0);
}

static int	whatsup(t_philo *philo)
{
	long	temp;

	temp = get_long(&philo->table->lock,
			&philo->table->dinner_is_over, philo->table);
	if (temp != 0)
		return (temp);
	temp = get_long(&philo->lock, &philo->rip, philo->table);
	if (temp != 0)
		return (temp);
	temp = get_long(&philo->table->lock,
			&philo->table->dinner_is_over, philo->table);
	if (temp != 0)
		return (temp);
	return (0);
}

static int	routine_loop(t_philo *philo)
{
	long	ret;

	while (1)
	{
		ret = whatsup(philo);
		if (ret != 0)
			return (ret < 0);
		if (eating_phase(philo))
			return (1);
		ret = whatsup(philo);
		if (ret != 0)
			return (ret < 0);
		if (printing_status(philo, SLEEP))
			return (1);
		if (sleeping(philo->table->t_sleep * 1e3,
				get_time(US, philo->table), philo->table))
			return (1);
		ret = whatsup(philo);
		if (ret != 0)
			return (ret < 0);
		if (thinking_phase(philo, false))
			return (1);
	}
	return (0);
}

void	*dinner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (before_start(philo))
		return ((void *)1);
	if (routine_loop(philo))
		return ((void *)1);
	return ((void *)0);
}
