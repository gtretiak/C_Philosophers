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
	void	*check;

        i = -1;
	check = NULL;
        philo_nbr = get_long(&cafe->table->lock, &cafe->table->n_philos);
	if (philo_nbr == -2)
		return (handle_error(cafe, 4, MUTEX));
	if (philo_nbr == 1)
	{
		if (pthread_create(&cafe->all_philos[0].philo_acting, NULL, run_alone, &cafe->all_philos[0]))
			return (handle_error(cafe, 4, THREAD));
	}
	else
        {
		while (++i < philo_nbr)
		{
			if (pthread_create(&cafe->all_philos[i].philo_acting, NULL,
                        dinner, &cafe->all_philos[i]))
				return (handle_error(cafe, 4, THREAD));
		}
        }
	if (set_long(&cafe->table->lock, &cafe->table->t_start, get_time()) == -1
		|| set_long(&cafe->table->lock, &cafe->table->all_ready, 1) == -1)
		return (handle_error(cafe, 4, MUTEX));
	if (pthread_create(&cafe->waiter, NULL, serving, &cafe))
		return (handle_error(cafe, 4, THREAD));
	i = -1;
	while (++i < philo_nbr)
	{
		pthread_join(cafe->all_philos[i].philo_acting, (void *)&check);
		if (*(int *)check)
		{
			free(check);
			return (handle_error(cafe, 4, THREAD));
		}
		free(check);
	}
	if (set_long(&cafe->table->lock, &cafe->table->dinner_is_over, 1) == -1)
		return (handle_error(cafe, 4, MUTEX));
        pthread_join(cafe->waiter, (void *)&check);
	if (*(int *)check)
	{
		free(check);
		return (handle_error(cafe, 4, THREAD));
	}
	free(check);
	return (0);
}

void	*run_alone(void *arg)
{
	t_philo *philo;
	int	*check;
	long	t_curr;
	long	the_end;
	
	check = malloc(sizeof(int));
	*check = 1;
	philo = (t_philo *)arg;
	if (wait_others(philo))
		return (check);
	if (mutex_handler(&philo->philo_lock, LOCK))
		return (check);
	philo->t_last_meal = get_time();
	if (philo->t_last_meal == -777)
		return (check);
	if (mutex_handler(&philo->philo_lock, UNLOCK))
		return (check);
	if (mutex_handler(&philo->table->lock, LOCK))
		return (check);
	philo->table->running_threads++;
	if (mutex_handler(&philo->table->lock, UNLOCK))
		return (check);
	if (printing_status(philo, FORK))
		return (check);
	while(1)
	{
		the_end = get_long(&philo->table->lock, &philo->table->dinner_is_over);
		if (the_end == -2)
			return (check);
		else if (the_end == 1)
			break ;
		t_curr = get_time();
		if (t_curr < 0)
			return (check);
		if (get_long(&philo->table->lock, &philo->table->t_die) < t_curr - get_long(&philo->philo_lock, &philo->t_last_meal))
		{
			if (mutex_handler(&philo->philo_lock, LOCK))
				return (check);
			philo->rip = true; // check and displaying within 10ms of actual death TODO
			if (mutex_handler(&philo->philo_lock, UNLOCK))
				return (check);
			if (printing_status(philo, DIE))
				return (check);
			*check = 0;
			return (NULL);
		}
	}
	*check = 0;
	return (NULL);
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

	while(1)
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
			return (1);
		if (mutex_handler(&philo->second_fork->mtx, LOCK))
			return (1);
		if (printing_status(philo, FORK))
			return (1);
		if (mutex_handler(&philo->philo_lock, LOCK))
			return (1);
		philo->t_last_meal = get_time();
		if (philo->t_last_meal == -666)
			return (1);
		if (mutex_handler(&philo->philo_lock, UNLOCK))
			return (1);
		if (printing_status(philo, EAT))
			return (1);
		if (precise_usleep(philo->table->t_eat, philo->table))
			return (1);
		if (mutex_handler(&philo->first_fork->mtx, UNLOCK))
			return (1);
		if (mutex_handler(&philo->second_fork->mtx, UNLOCK))
			return (1);
		meals_nbr = get_long(&philo->table->lock, &philo->table->n_meals);
		if (meals_nbr == -2)
			return (1);
		if (meals_nbr != -1 
			&& ++philo->meals_eaten >= meals_nbr) // REPLACE WITH INCREASE FUNCTION
		{
			if (mutex_handler(&philo->philo_lock, LOCK))
				return (1);
			philo->full = 1;
			if (mutex_handler(&philo->philo_lock, UNLOCK))
				return (1);
		}
	}
	return (0);
}

/* STOPPED HERE*/
void	*dinner(void *arg)
{
	t_philo	*philo;
	long	t_curr;
	int	*check;

	philo = (t_philo *)arg;
	check = malloc(sizeof(int));
	*check = 1;
	wait_others(philo);
	if (mutex_handler(&philo->table->lock, LOCK))
		return (check);
	philo->table->running_threads++;
	if (mutex_handler(&philo->table->lock, UNLOCK))
		return (check);
	if (think_about(philo))
		return (check);
	while (!philo->table->dinner_is_over)
	{
		t_curr = get_time();
		if (t_curr < 0)
			return (check);
		if (philo->table->t_die < t_curr - philo->t_last_meal)
		{
			philo->rip = true; // check and displaying within 10ms of actual death TODO
			if (printing_status(philo, DIE))
				return (check);
			free(check);
			return (NULL);
		}
		if (eating_phase(philo))
			return (check);
		if (printing_status(philo, SLEEP))
			return (check);
		if (precise_usleep(philo->table->t_sleep, philo->table))
			return (check);
		t_curr = get_time();
	}
	*check = 0;
	return (NULL);
}

void    *serving(void *arg)
{
        t_data  *cafe;
	int	iter;
	int	i;
	int	*check;	

       	cafe = (t_data *)arg;
	check = malloc(4);
	*check = 1;
	while (all_running(cafe->table) != 1)
		;
	iter = cafe->table->n_philos;
	while (!cafe->table->dinner_is_over)
	{
		i = -1;
		while (++i < iter && !cafe->table->dinner_is_over)
		{
			if (cafe->all_philos[i].rip)
			{
				if (mutex_handler(&cafe->table->lock, LOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return (check);
				}
				cafe->table->dinner_is_over = 1;
				if (mutex_handler(&cafe->table->lock, UNLOCK))
				{
					handle_error(cafe, 4, MUTEX);
					return (check);
				}
				return (NULL);
			}
			else if (cafe->all_philos[i].full)
			{
				if (++cafe->table->n_full >= iter)
				{
					if (mutex_handler(&cafe->table->lock, LOCK))
					{
						handle_error(cafe, 4, MUTEX);
						return (check);
					}
					cafe->table->dinner_is_over = 1;
					if (mutex_handler(&cafe->table->lock, UNLOCK))
					{
						handle_error(cafe, 4, MUTEX);
						return (check);
					}
					return (NULL);
				}
			}
		}
	}
	if (mutex_handler(&cafe->table->lock, LOCK))
	{
		handle_error(cafe, 4, MUTEX);
		return (check);
	}
	cafe->table->dinner_is_over = 1;
	if (mutex_handler(&cafe->table->lock, UNLOCK))
	{
		handle_error(cafe, 4, MUTEX);
		return (check);
	}
	return (NULL);
}
