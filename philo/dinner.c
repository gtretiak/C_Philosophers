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
	long		philo_nbr;
	void		*ret;

	i = -1;
	philo_nbr = get_long(&cafe->table->lock, &cafe->table->n_philos, cafe->table);
	if (philo_nbr == -2)
		return (cleanup(35, philo_nbr * 2 + 2, NULL, cafe));
	//thread creation
	if (philo_nbr == 1)
	{
		i = 1;
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
	if (pthread_create(&cafe->waiter, NULL, serving, cafe))
		return (cleanup(11, i, THREAD, cafe));
	//preparation
	if (set_long(&cafe->table->lock, &cafe->table->t_start,
		get_time(MS, cafe->table), cafe->table)
		|| set_long(&cafe->table->lock, &cafe->table->all_ready, 1, cafe->table))
		return (cleanup(11, i + 1, NULL, cafe));
	//joining (do I need to join the waiter last? If so, redo i & j)
	if (pthread_join(cafe->waiter, (void *)&ret))
		return (cleanup(11, i, THREAD, cafe));
	else if (ret != (void *)0)
		return (cleanup(11, i, NULL, cafe));
	if (set_long(&cafe->table->lock, &cafe->table->dinner_is_over, 1, cafe->table))
		return (cleanup(11, i, THREAD, cafe));
	if (philo_nbr == 1)
	{
		if (pthread_join(cafe->philos[0].th, (void *)&ret))
			return (cleanup(11, 0, THREAD, cafe));
		else if (ret != (void *)0)
			return (cleanup(11, 0, NULL, cafe));
	}
	else
	{
		while (--i >= 0)
		{
			if (pthread_join(cafe->philos[i].th, (void *)&ret))
				return (cleanup(11, i, THREAD, cafe));
			else if (ret != (void *)0)
				return (cleanup(11, i, NULL, cafe));
		}
	}
	return (0);
}

void	*run_alone(void *arg)
{
	t_philo	*philo;
	long	temp;

	philo = (t_philo *)arg;
	if (wait_others(philo))
		return ((void *)1);
	if (increase_long(&philo->table->lock, &philo->table->running_threads, philo->table))
		return ((void *)1);
	if (set_long(&philo->lock, &philo->t_last_meal, get_time(MS, philo->table), philo->table))
		return ((void *)1);
	//implement taking the fork
	if (set_long(&philo->fork1->lock, &philo->fork1->taken, 1, philo->table))
		return ((void *)1);
	if (printing_status(philo, FORK))
		return ((void *)1);
	while (1)
	{
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
		if (temp == 1)
			return ((void *)0);
		else if (temp < 0)
			return ((void *)1);
		temp = get_long(&philo->lock, &philo->rip, philo->table);
		if (temp < 0)
			return ((void *)1);
		else if (temp == 1)
			return ((void *)0);		
	}
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
	philos_nbr = get_long(&philo->table->lock, &philo->table->n_philos, philo->table);
	if (philos_nbr == -2)
		return (1);
	if (philos_nbr % 2 == 0)
		return (0);
	t_to_eat = get_long(&philo->table->lock, &philo->table->t_eat, philo->table);
	t_to_sleep = get_long(&philo->table->lock, &philo->table->t_sleep, philo->table);
	t_think = t_to_eat * 2 - t_to_sleep;
	if (t_to_eat == -2 || t_to_sleep == -2)
		return (1);
	if (t_think < 0)
		t_think = 0;
	if (sleeping(t_think * 0.5, get_time(US, philo->table), philo->table))
		return (1);
	return (0);
}

int	think_about(t_philo *philo)
{
	if (philo->table->n_philos % 2 == 0)
	{
		if (philo->pos % 2 == 0)
		{
			if (sleeping(3e4, get_time(US, philo->table), philo->table))
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
	long	temp;
	long	meals_nbr;
	long	time;
	long	eaten;

//	while (1)
//	{
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
		if (temp == 1)
			return (0);
			//break ;
		else if (temp < 0)
			return (1);
		if (set_long(&philo->fork1->lock, &philo->fork1->taken, 1, philo->table))
			return (1);
//		if (handle_mtx(&philo->fork1->lock, LOCK, philo->table))
//			return (1);
		if (printing_status(philo, FORK))
			return (1);
//		if (handle_mtx(&philo->fork2->lock, LOCK, philo->table))
//			return (1);
		temp = get_long(&philo->fork2->lock, &philo->fork2->taken, philo->table);
		if (temp < 0)
			return (1);
		else if (temp == 1)
		{
			if (set_long(&philo->fork1->lock, &philo->fork1->taken, 0, philo->table))
				return (1);
			return (0);
			//continue ;
		}
		if (set_long(&philo->fork2->lock, &philo->fork2->taken, 1, philo->table))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		//eating
		time = get_time(MS, philo->table);
		if (time < 0 || set_long(&philo->lock, &philo->t_last_meal, get_time(MS, philo->table), philo->table))
			return (1);
		if (printing_status(philo, EAT))
			return (1);
		if (sleeping(philo->table->t_eat * 1e3, get_time(US, philo->table), philo->table))
			return (1);
		//releasing forks
		/*if (handle_mtx(&philo->fork1->lock, UNLOCK, philo->table))
			return (1);
		if (handle_mtx(&philo->fork2->lock, UNLOCK, philo->table))
			return (1);*/
		if (set_long(&philo->fork1->lock, &philo->fork1->taken, 0, philo->table))
			return (1);
		if (set_long(&philo->fork2->lock, &philo->fork2->taken, 0, philo->table))
			return (1);
		//updating data
		meals_nbr = get_long(&philo->table->lock, &philo->table->n_meals, philo->table);
		if (meals_nbr == -2 || increase_long(&philo->lock, &philo->meals_eaten, philo->table))
			return (1);
		eaten = get_long(&philo->lock, &philo->meals_eaten, philo->table);
		if (eaten < 0)
			return (1);
		if (meals_nbr != -1 && eaten >= meals_nbr)
		{
			if (set_long(&philo->lock, &philo->full, 1, philo->table))
				return (1);
		}
		//break ;
	return (0);
}

void	*dinner(void *arg)
{
	t_philo	*philo;
	long	temp;

	philo = (t_philo *)arg;
	if (wait_others(philo))
		return ((void *)1);
	if (increase_long(&philo->table->lock, &philo->table->running_threads, philo->table))
		return ((void *)1);
	if (set_long(&philo->lock, &philo->t_last_meal, get_time(MS, philo->table), philo->table))
		return ((void *)1);
	//think
	if (think_about(philo))
		return ((void *)1);
	while (1)
	{
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
	       	if (temp < 0)
			return ((void *)1);
		else if (temp > 0)
			break ;
		temp = get_long(&philo->lock, &philo->rip, philo->table);
		if (temp < 0)
			return ((void *)1);
		else if (temp == 1)
			return ((void *)0);
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
	       	if (temp < 0)
			return ((void *)1);
		else if (temp == 1)
			return ((void *)0);
		//eat
		if (eating_phase(philo))
			return ((void *)1);
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
	       	if (temp < 0)
			return ((void *)1);
		else if (temp > 0)
			break ;
		//sleep
		if (printing_status(philo, SLEEP))
			return ((void *)1);
		if (sleeping(philo->table->t_sleep * 1e3, get_time(US, philo->table), philo->table))
			return ((void *)1);
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over, philo->table);
	       	if (temp < 0)
			return ((void *)1);
		else if (temp > 0)
			break ;
		if (thinking_phase(philo, false))
			return ((void *)1);
	}
	return ((void *)0);
}

static int	is_philo_dead(t_philo *philo)
{
	long	t_curr;
	long	temp;

	t_curr = get_time(MS, philo->table);
	if (t_curr < 0)
		return (-2);
	temp = get_long(&philo->lock, &philo->t_last_meal, philo->table);
	if (temp == -2)
		return (-2);
	t_curr -= temp;
	temp = get_long(&philo->table->lock, &philo->table->t_die, philo->table);
	if (temp == -2)
		return (-2);
	if (temp < t_curr)
	{
		if (printing_status(philo, DIE))
			return (-2);
		if (set_long(&philo->lock, &philo->rip, 1, philo->table))
			return (-2);
		if (set_long(&philo->table->lock, &philo->table->dinner_is_over, 1, philo->table))
			return (-2);
		return (1);
	}
	return (0);
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
	iter = get_long(&cafe->table->lock, &cafe->table->n_philos, cafe->table);
	if (iter == -2)
		return ((void *)1);
	while (1)
	{
		i = -1;
		while (++i < iter
			&& get_long(&cafe->table->lock, &cafe->table->dinner_is_over, cafe->table)
			== 0 && get_long(&cafe->table->lock, &cafe->table->n_full, cafe->table) < iter)
		{
			temp = is_philo_dead(&cafe->philos[i]);
			if (temp < 0)
				return ((void *)1);
			else if (temp == 1)
				return ((void *)0);
			// if philo is full? -> increase and see if all the full
			// if so -> dinner is over
			temp = get_long(&cafe->philos[i].lock, &cafe->philos[i].full, cafe->table);
			if (temp < 0)
				return ((void *)1);
			else if (temp == 1)
			{
				if (increase_long(&cafe->table->lock, &cafe->table->n_full, cafe->table))
					return ((void *)1);
				temp = get_long(&cafe->table->lock, &cafe->table->n_full, cafe->table);
				if (temp < 0)
					return ((void *)1);
				if (temp >= iter)
				{
					if (set_long(&cafe->table->lock,
							&cafe->table->dinner_is_over, 1, cafe->table))
						return ((void *)1);
					return ((void *)0);
				}
			/*	printf("Checking if %ld died (last meal: %ld, now: %ld)\n",
					cafe->philos[i].pos, cafe->philos[i].t_last_meal,
					get_time(MS));*/
			}
		}
		sleeping(1e3, get_time(US, cafe->table), cafe->table);
	}
	return ((void *)0);
}
