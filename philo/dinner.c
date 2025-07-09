/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/09 19:23:36 by gtretiak         ###   ########.fr       */
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
	//thread creation
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
	//preparation
	if (set_long(&cafe->table->lock, &cafe->table->t_start, get_time(MS))
		|| set_long(&cafe->table->lock, &cafe->table->all_ready, 1))
		return (cleanup(11, i, NULL, cafe));
	//joining (do I need to join the waiter last? If so, redo i & j)
	if (pthread_join(cafe->waiter, (void *)&ret))
		return (cleanup(11, i - 1, THREAD, cafe));
	else if (ret != (void *)0)
		return (cleanup(11, i, NULL, cafe));
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
	if (wait_others(philo))
		return ((void *)1);
	if (increase_long(&philo->table->lock, &philo->table->running_threads))
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

int	thinking_phase(t_philo *philo, bool preset)
{
	long	t_think;
	long	t_to_eat;
	long	t_to_sleep;
	long	philos_nbr;

	if (!preset)
	{
		if (printing_status(philo, THINK))
			return (1);
	}
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
			if (thinking_phase(philo, true)) 
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

	the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	if (the_end == 0)
	{
		// taking forks + checking if forks are available
		if (set_bool(&philo->fork1->lock, &philo->fork1->taken, true))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		if (set_bool(&philo->fork2->lock, &philo->fork2->taken, true))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		//eating
		time = get_time(MS);
		if (!time || set_long(&philo->lock, &philo->t_last_meal, get_time(MS)))
			return (1);
		if (printing_status(philo, EAT))
			return (1);
		if (sleeping(philo->table->t_eat * 1e3, get_time(US), philo->table))
			return (1);
		//releasing forks
		if (set_bool(&philo->fork1->lock, &philo->fork1->taken, false))
			return (1);
		if (set_bool(&philo->fork2->lock, &philo->fork2->taken, false))
			return (1);
		//updating data
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
	else if (the_end == -2)
		return (1);
	return (0);
}

void	*dinner(void *arg)
{
	t_philo	*philo;
	long	t_curr;
	long	temp;

	philo = (t_philo *)arg;
	if (wait_others(philo))
		return ((void *)1);
	if (increase_long(&philo->table->lock, &philo->table->running_threads))
		return ((void *)1);
	if (set_long(&philo->lock, &philo->t_last_meal, get_time(MS)))
		return ((void *)1);
	//think
	if (think_about(philo))
		return ((void *)1);
	while (1)
	{
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over);
	       	if (temp < 0)
			return ((void *)1);
		else if (temp > 0)
			break ;
		//am I dead?
		t_curr = get_time(MS);
		if (t_curr < 0)
			return ((void *)1);
		temp = get_long(&philo->lock, &philo->t_last_meal);
		if (temp == -2)
			return ((void *)1);
		t_curr -= temp;
		temp = get_long(&philo->table->lock, &philo->table->t_die);
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
		//eat
		if (eating_phase(philo))
			return ((void *)1);
		//sleep
		if (printing_status(philo, SLEEP))
			return ((void *)1);
		if (sleeping(philo->table->t_sleep * 1e3, get_time(US), philo->table))
			return ((void *)1);
		if (thinking_phase(philo, false))
			return ((void *)1);
	}
	return ((void *)0);
}

void	*serving(void *arg)
{
	t_data	*cafe;
	int		iter;
	int		i;
	long	temp;

	cafe = (t_data *)arg;
	while (1)
	{
		i = all_running(cafe->table);
	       	if (i == 1)
			break ;
		else if (i < 0)
			return ((void *)1);
		;
	}
	iter = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (iter == -2)
		return ((void *)1);
	while (1)
	{
		i = -1;
		while (++i < iter
			&& get_long(&cafe->table->lock, &cafe->table->dinner_is_over)
			== 0 && get_long(&cafe->table->lock, &cafe->table->n_full) < iter)
		{
			//if philo is dead? -> dinner is over
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
			// if philo is full? -> increase and see if all the full
			// if so -> dinner is over
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
			/*	printf("Checking if %ld died (last meal: %ld, now: %ld)\n",
					cafe->philos[i].pos, cafe->philos[i].t_last_meal,
					get_time(MS));*/
			}
		}
	}
	return ((void *)0);
}
