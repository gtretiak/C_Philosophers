/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 19:06:31 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int    run_simulation(t_data *cafe)
{
        int     i;
	long	philo_nbr;
	long	the_end;

        i = -1;
        philo_nbr = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (philo_nbr == -2)
		return (handle_error(cafe, 4, MUTEX));
	if (philo_nbr == 1)
	{
		if (pthread_create(&cafe->all_philos[0].philo_acting, NULL, run_alone, &cafe->all_philos[0]))
			return (handle_error(cafe, 5, THREAD));
	}
	else
        {
		while (++i < philo_nbr)
		{
			if (pthread_create(&cafe->all_philos[i].philo_acting, NULL,
                        dinner, &cafe->all_philos[i]))
				return (handle_error(cafe, 5, THREAD));
		}
        }
	if (set_long(&cafe->table->lock, &cafe->table->t_start, get_time()) == -1
		|| set_long(&cafe->table->lock, &cafe->table->all_ready, 1) == -1)
		return (handle_error(cafe, 5, MUTEX));
	if (pthread_create(&cafe->waiter, NULL, serving, &cafe))
		return (handle_error(cafe, 5, THREAD));
	while (1)
	{
		the_end = get_long(&cafe->table->lock, &cafe->table->dinner_is_over);
		if (the_end == -2)
			return (handle_error(cafe, 5, MUTEX));
		else if (the_end == 1)
			break ;
	}
	return (0);
}

void	*run_alone(void *arg)
{
	t_philo *philo;
	long	t_curr;
	long	the_end;
	
	philo = (t_philo *)arg;
	if (wait_others(philo))
		return ((void *)1);
	if (mutex_handler(&philo->philo_lock, LOCK))
		return ((void *)1);
	philo->t_last_meal = get_time();
	if (philo->t_last_meal == -777)
	{
		mutex_handler(&philo->philo_lock, UNLOCK);
		return ((void *)1);
	}
	if (mutex_handler(&philo->philo_lock, UNLOCK))
		return ((void *)1);
	if (mutex_handler(&philo->table->lock, LOCK))
		return ((void *)1);
	philo->table->running_threads++;
	if (mutex_handler(&philo->table->lock, UNLOCK))
		return ((void *)1);
	if (printing_status(philo, FORK))
		return ((void *)1);
	while(1)
	{
		the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
		if (the_end == -2)
			return ((void *)1);
		else if (the_end == 1)
			break ;
		t_curr = get_time();
		if (t_curr < 0)
			return ((void *)1);
		if (get_long(&philo->table->lock, &philo->table->t_die) < t_curr - get_long(&philo->philo_lock, &philo->t_last_meal))
		{
			if (mutex_handler(&philo->philo_lock, LOCK))
				return ((void *)1);
			philo->rip = true; // check and displaying within 10ms of actual death TODO
			if (mutex_handler(&philo->philo_lock, UNLOCK))
				return ((void *)1);
			if (printing_status(philo, DIE))
				return ((void *)1);
			return ((void *)0);
		}
	}
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
	if (precise_usleep(t_think * 0.5, philo->table))
		return (1);
	return (0);
}

int	think_about(t_philo *philo)
{
	if (philo->table->n_philos % 2 == 0)
	{
		if (philo->position % 2 == 0)
		{
			if (precise_usleep(3e4, philo->table))
				return (1);
		}
	}
	else
	{
		if (philo->position % 2 != 0)
			if (thinking_phase(philo))
				return (1);
	}
	return (0);
}

int	eating_phase(t_philo *philo)
{
	long	the_end;
	long	meals_nbr;

	while (1)
	{
		the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
		if (the_end == -2)
			return (1);
		else if (the_end == 1)
			break ;
	/*	if (!philo->allowed_to_eat)
		{
			if (precise_usleep(60000, philo->table))
				return (1);
			continue ;
		}*/
		if (mutex_handler(&philo->first_fork->mtx, LOCK))
			return (1);
		if (printing_status(philo, FORK))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			return (1);
		}
		if (mutex_handler(&philo->second_fork->mtx, LOCK))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			return (1);
		}
		if (printing_status(philo, FORK))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (mutex_handler(&philo->philo_lock, LOCK))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		philo->t_last_meal = get_time();
		if (philo->t_last_meal == -666)
		{
			mutex_handler(&philo->philo_lock, UNLOCK);
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (mutex_handler(&philo->philo_lock, UNLOCK))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (printing_status(philo, EAT))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (precise_usleep(philo->table->t_eat, philo->table))
		{
			mutex_handler(&philo->first_fork->mtx, UNLOCK);
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (mutex_handler(&philo->first_fork->mtx, UNLOCK))
		{
			mutex_handler(&philo->second_fork->mtx, UNLOCK);
			return (1);
		}
		if (mutex_handler(&philo->second_fork->mtx, UNLOCK))
			return (1);
		meals_nbr = get_long(&philo->table->lock, &philo->table->n_meals);
		if (meals_nbr == -2)
			return (1);
		if (mutex_handler(&philo->philo_lock, LOCK))
			return (1);
		if (meals_nbr != -1 
			&& ++philo->meals_eaten >= meals_nbr)
			philo->full = 1;
		if (mutex_handler(&philo->philo_lock, UNLOCK))
			return (1);
		}
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
	if (mutex_handler(&philo->table->lock, LOCK))
		return ((void *)1);
	philo->table->running_threads++;
	if (mutex_handler(&philo->table->lock, UNLOCK))
		return ((void *)1);
	if (think_about(philo))
		return ((void *)1);
	while (1)
	{
		temp = get_long(&philo->table->lock, &philo->table->dinner_is_over);
		if (temp == -2)
			return ((void *)1);
		else if (temp == 1)
			break ;
		t_curr = get_time();
		if (t_curr < 0)
			return ((void *)1);
		temp = get_long(&philo->table->lock, &philo->t_last_meal);
		if (temp == -2)
			return ((void *)1);
		t_curr -= temp;
		temp = get_long(&philo->table->lock, &philo->table->t_die);
		if (temp == -2)
			return ((void *)1);
		if (temp < t_curr)
		{
			if (mutex_handler(&philo->table->lock, LOCK))
				return ((void *)1);
			philo->rip = true; // check and displaying within 10ms of actual death TODO
			if (mutex_handler(&philo->table->lock, UNLOCK))
				return ((void *)1);
			if (printing_status(philo, DIE))
				return ((void *)1);
			return ((void *)0);
		}
		if (eating_phase(philo))
			return ((void *)1);
		if (printing_status(philo, SLEEP))
			return ((void *)1);
		if (precise_usleep(philo->table->t_sleep, philo->table))
			return ((void *)1);
	}
	return ((void *)0);
}

void    *serving(void *arg)
{
        t_data  *cafe;
	int	iter;
	int	i;
	long	temp;

       	cafe = (t_data *)arg;
	while (all_running(cafe->table) != 1)
		;
	iter = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (iter == -2)
		return ((void *)1);
	while (1)
	{
		temp = get_long(&cafe->table->lock, &cafe->table->dinner_is_over);
		if (temp == -2)
			return ((void *)1);
		else if (temp == 1)
			break ;
		i = -1;
		while (++i < iter)
		{
			temp = get_long(&cafe->table->lock, &cafe->table->dinner_is_over);
			if (temp == -2)
				return ((void *)1);
			else if (temp == 1)
				break ;
			if (mutex_handler(&cafe->common_lock, LOCK))
			{
				handle_error(cafe, 4, MUTEX);
				return ((void *)1);
			}
			if (mutex_handler(&cafe->all_philos[i].philo_lock, LOCK))
			{
				handle_error(cafe, 4, MUTEX);
				return ((void *)1);
			}
			if (cafe->all_philos[i].rip)
			{
				if (mutex_handler(&cafe->all_philos[i].philo_lock, UNLOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return ((void *)1);
				}
				if (mutex_handler(&cafe->table->lock, LOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return ((void *)1);
				}
				cafe->table->dinner_is_over = 1;
				if (mutex_handler(&cafe->table->lock, UNLOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return ((void *)1);
				}
				return ((void *)0);
			}
			else if (cafe->all_philos[i].full)
			{
				if (mutex_handler(&cafe->table->lock, LOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return ((void *)1);
				}
				if (++cafe->table->n_full >= iter)
				{
					cafe->table->dinner_is_over = 1;
					return ((void *)0);
				}
				if (mutex_handler(&cafe->table->lock, UNLOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return ((void *)1);
				}
			}
			if (mutex_handler(&cafe->common_lock, UNLOCK))
			{
				handle_error(cafe, 4, MUTEX);
				return ((void *)1);
			}
		}
	}
	if (mutex_handler(&cafe->table->lock, LOCK))
	{
		handle_error(cafe, 4, MUTEX);
		return ((void *)1);
	}
	cafe->table->dinner_is_over = 1;
	if (mutex_handler(&cafe->table->lock, UNLOCK))
	{
		handle_error(cafe, 4, MUTEX);
		return ((void *)1);
	}
	return ((void *)0);
}
