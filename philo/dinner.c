/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/09 15:05:04 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	run_simulation(t_data *cafe)
{
	int			i;
	int			j;
	long		philo_nbr;
	void		*ret;

	i = -1;
	philo_nbr = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (philo_nbr == -2)
		return (cleanup(35, philo_nbr * 2 + 2, NULL, cafe));
	if (philo_nbr == 1)
	{
		if (pthread_create(&cafe->philos[0].th, NULL,
				run_alone, &cafe->philos[0]))
			return (cleanup(11, 0, THREAD, cafe));
	}
	else
	{
		while (++i < philo_nbr)
		{
			if (pthread_create(&cafe->philos[i].th, NULL,
					dinner, &cafe->philos[i]))
				return (cleanup(11, i, THREAD, cafe));
		}
	}
	if (pthread_create(&cafe->waiter, NULL, serving, &cafe))
		return (cleanup(11, i - 1, THREAD, cafe));
	if (set_long(&cafe->table->lock, &cafe->table->t_start, get_time(MS))
		|| set_long(&cafe->table->lock, &cafe->table->all_ready, 1))
		return (cleanup(11, i, NULL, cafe));
	if (pthread_join(cafe->waiter, (void *)&ret))
		return (cleanup(11, i - 1, THREAD, cafe));
	else if (ret != (void *)0)
		return (cleanup(11, i - 1, NULL, cafe));
	j = i - 2;
	if (philo_nbr == 1)
	{
		if (pthread_join(cafe->philos[0].th, (void *)&ret))
			return (cleanup(11, j, THREAD, cafe));
		else if (ret != (void *)0)
			return (cleanup(11, j, NULL, cafe));
	}
	else
	{
		i = -1;
		while (++i < philo_nbr)
		{
			if (pthread_join(cafe->philos[i].th, (void *)&ret))
				return (cleanup(11, j - i, THREAD, cafe));
			else if (ret != (void *)0)
				return (cleanup(11, j - i, NULL, cafe));
		}
	}
	return (0);
}

void	*run_alone(void *arg)
{
	t_philo	*philo;
	long	t_curr;
	long	the_end;

	philo = (t_philo *)arg;
	if (increase_long(&philo->table->lock, &philo->table->running_threads))
		return ((void *)1);
	if (wait_others(philo))
		return ((void *)1);
	if (set_long(&philo->lock, &philo->t_last_meal, get_time(MS)))
		return ((void *)1);
	if (printing_status(philo, FORK))
		return ((void *)1);
	while (get_long(&philo->table->lock, &philo->table->dinner_is_over) == 0)
	{
		t_curr = get_time(MS);
		if (t_curr < 0)
			return ((void *)1);
		if (get_long(&philo->table->lock, &philo->table->t_die)
			<= t_curr - get_long(&philo->lock, &philo->t_last_meal))
		{
			if (set_long(&philo->lock, &philo->rip, 1))
				return ((void *)1);
			if (printing_status(philo, DIE))
				return ((void *)1);
			return ((void *)0);
		}
	}
	the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	if (the_end == -2)
		return ((void *)1);
	return ((void *)0);
}

int	thinking_phase(t_philo *philo)
{
	long	t_think;
	long	t_to_eat;
	long	t_to_sleep;
	long	philos_nbr;

	if (printing_status(philo, THINK))
		return (1);
	philos_nbr = get_long(&philo->table->lock, &philo->table->n_philos);
	if (philos_nbr == -2)
		return (1);
	if (philos_nbr % 2 == 0)
		return (0);
	t_to_eat = get_long(&philo->table->lock, &philo->table->t_eat);
	t_to_sleep = get_long(&philo->table->lock, &philo->table->t_sleep);
	t_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_eat == -2 || t_to_sleep == -2)
		return (1);
	if (t_think < 0)
		t_think = 0;
	if (sleeping(t_think * 0.5, get_time(US), philo->table))
		return (1);
	return (0);
}

int	think_about(t_philo *philo)
{
	if (philo->table->n_philos % 2 == 0)
	{
		if (philo->pos % 2 == 0)
		{
			if (sleeping(3e4, get_time(US), philo->table))
				return (1);
		}
	}
	else
	{
		if (philo->pos % 2 != 0)
			if (thinking_phase(philo))
				return (1);
	}
	return (0);
}

int	eating_phase(t_philo *philo)
{
	long	the_end;
	long	meals_nbr;
	long	time;
	long	eaten;

	if (get_long(&philo->table->lock, &philo->table->dinner_is_over) == 0)
	{
		if (set_bool(&philo->fork1->lock, &philo->fork1->taken, true))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		if (set_bool(&philo->fork2->lock, &philo->fork2->taken, true))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		time = get_time(MS);
		if (!time || set_long(&philo->lock, &philo->t_last_meal, get_time(MS)))
			return (1);
		if (printing_status(philo, EAT))
			return (1);
		if (sleeping(philo->table->t_eat * 1e3, get_time(US), philo->table))
			return (1);
		if (set_bool(&philo->fork1->lock, &philo->fork1->taken, false))
			return (1);
		if (set_bool(&philo->fork2->lock, &philo->fork2->taken, false))
			return (1);
		meals_nbr = get_long(&philo->table->lock, &philo->table->n_meals);
		if (meals_nbr == -2 || increase_long(&philo->lock, &philo->meals_eaten))
			return (1);
		eaten = get_long(&philo->lock, &philo->meals_eaten);
		if (eaten < 0)
			return (1);
		if (meals_nbr != -1 && eaten >= meals_nbr)
		{
			if (set_long(&philo->lock, &philo->full, 1))
				return (1);
		}
	}
	the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	if (the_end == -2)
		return (1);
	return (0);
}

void	*dinner(void *arg)
{
	t_philo	*philo;
	long	t_curr;
	long	temp;

	philo = (t_philo *)arg;
	if (increase_long(&philo->table->lock, &philo->table->running_threads))
		return ((void *)1);
	if (wait_others(philo))
		return ((void *)1);
	if (set_long(&philo->lock, &philo->t_last_meal, get_time(MS)))
		return ((void *)1);
	if (think_about(philo))
		return ((void *)1);
	while (get_long(&philo->table->lock, &philo->table->dinner_is_over) == 0)
	{
		t_curr = get_time(MS);
		if (t_curr < 0)
			return ((void *)1);
		temp = get_long(&philo->lock, &philo->t_last_meal);
		if (temp == -2)
			return ((void *)1);
		t_curr -= temp;
		temp = get_long(&philo->table->lock, &philo->table->t_die);
		printf("Time passed:%ld\nTime to die:%ld\n", t_curr, temp);
		if (temp == -2)
			return ((void *)1);
		if (temp < t_curr)
		{
			if (set_long(&philo->table->lock, &philo->rip, 1))
				return ((void *)1);
			if (printing_status(philo, DIE))
				return ((void *)1);
			return ((void *)0);
		}
		if (eating_phase(philo))
			return ((void *)1);
		printf("Philo %ld. Time:%ld\nLast meal:%ld\n",
			philo->pos, t_curr, temp);
		if (printing_status(philo, SLEEP))
			return ((void *)1);
		if (sleeping(philo->table->t_sleep * 1e3, get_time(US), philo->table))
			return ((void *)1);
	}
	temp = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	if (temp == -2)
		return ((void *)1);
	return ((void *)0);
}

void	*serving(void *arg)
{
	t_data	*cafe;
	int		iter;
	int		i;
	long	temp;

	cafe = (t_data *)arg;
	printf("Serving thread started\n");
	while (all_running(cafe->table) != 1)
		sleeping(1000, get_time(US), cafe->table);
	iter = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (iter == -2)
		return ((void *)1);
	while (get_long(&cafe->table->lock, &cafe->table->dinner_is_over)
		== 0 && get_long(&cafe->table->lock, &cafe->table->n_full) < iter)
	{
		i = -1;
		while (++i < iter
			&& get_long(&cafe->table->lock, &cafe->table->dinner_is_over)
			== 0 && get_long(&cafe->table->lock, &cafe->table->n_full) < iter)
		{
			temp = get_long(&cafe->philos[i].lock, &cafe->philos[i].rip);
			if (temp < 0)
				return ((void *)1);
			else if (temp == 1)
			{
				if (set_long(&cafe->table->lock,
						&cafe->table->dinner_is_over, 1))
					return ((void *)1);
				return ((void *)0);
			}
			temp = get_long(&cafe->philos[i].lock, &cafe->philos[i].full);
			if (temp < 0)
				return ((void *)1);
			else if (temp == 1)
			{
				if (increase_long(&cafe->table->lock, &cafe->table->n_full))
					return ((void *)1);
				temp = get_long(&cafe->table->lock, &cafe->table->n_full);
				if (temp < 0)
					return ((void *)1);
				if (temp >= iter)
				{
					if (set_long(&cafe->table->lock,
							&cafe->table->dinner_is_over, 1))
						return ((void *)1);
					return ((void *)0);
				}
				printf("Checking if %ld died (last meal: %ld, now: %ld)\n",
					cafe->philos[i].pos, cafe->philos[i].t_last_meal,
					get_time(MS));
				fflush(stdout);
			}
		}
	}
	temp = set_long(&cafe->table->lock, &cafe->table->dinner_is_over, 1);
	if (temp == -2)
		return ((void *)1);
	return ((void *)0);
}
