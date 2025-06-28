/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:48:53 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/28 20:11:31 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    run_simulation(t_data *cafe)
{
        int     i;

        i = -1;
	mutex_handler(&cafe->table->lock, LOCK, cafe);
	gettimeofday(&cafe->table->t_start, NULL);
	mutex_handler(&cafe->table->lock, UNLOCK, cafe);
	thread_handler(&cafe->waiter, CREATE, serving, cafe);
        if (cafe->table->n_philos == 1)
		;
        //        run_one(); // TODO
        else
        {
		printf("Simulation has started at %lu\n", cafe->table->t_start.tv_usec);
		while (++i < cafe->table->n_philos)
		{
			if (pthread_create(&cafe->all_philos[i].philo_acting, NULL,
                        dinner, &cafe->all_philos[i]))
				handle_error(cafe, 4, THREAD);
		}
		mutex_handler(&cafe->table->lock, LOCK, cafe);
		cafe->table->all_ready = true;
		mutex_handler(&cafe->table->lock, UNLOCK, cafe);
        }
}

void	*dinner(void *arg)
{
	t_philo	*philo;
        struct timeval	last_meal;
	struct timeval	curr_time;

	philo = (t_philo *)arg;
	wait_others(philo);
	while (!philo->table->dinner_is_over)
	{
		if (!philo->allowed_to_eat)
		{
			usleep(60000);
			continue ;
		}
		mutex_handler(&philo->first_fork->mtx, LOCK, NULL);
		gettimeofday(&curr_time, NULL);
		printf("%lu %d %s", curr_time.tv_usec, philo->position, FORK);
		mutex_handler(&philo->first_fork->mtx, UNLOCK, NULL);
		mutex_handler(&philo->second_fork->mtx, LOCK, NULL);
		gettimeofday(&curr_time, NULL);
		printf("%lu %d %s", curr_time.tv_usec, philo->position, FORK);
		mutex_handler(&philo->second_fork->mtx, UNLOCK, NULL);
		printf("%lu %d %s", curr_time.tv_usec, philo->position, EAT);
		gettimeofday(&last_meal, NULL);
		philo->t_last_meal = last_meal.tv_usec;
		usleep(philo->table->t_eat);
		if (philo->table->n_meals != -1
			&& ++philo->meals_eaten >= philo->table->n_meals)
		{
			philo->full = true;
			break ;
		}
		gettimeofday(&curr_time, NULL);
		printf("%lu %d %s", curr_time.tv_usec, philo->position, SLEEP);
		usleep(philo->table->t_sleep);
		gettimeofday(&curr_time, NULL);
		printf("%lu %d %s", curr_time.tv_usec, philo->position, THINK);
		gettimeofday(&curr_time, NULL);
		if (philo->table->t_die < curr_time.tv_usec - philo->t_last_meal)
		{
			philo->rip = true; // check and displaying within 10ms of actual death TODO
			gettimeofday(&curr_time, NULL);
			printf("%lu %d %s", curr_time.tv_usec, philo->position, DIED);
			return (NULL);
		}
	}
	return (NULL);
}

void    *serving(void *arg)
{
        t_data  *cafe;
	int	iter;
	int	i;	

        cafe = (t_data *)arg;
	iter = cafe->table->n_philos;
	i = -1;
	printf("I'm a waiter\n");
	while (iter && !cafe->table->dinner_is_over)
	{
		while (++i < iter)
		{
			if (cafe->all_philos[i].rip)
			{
				mutex_handler(&cafe->table->lock, LOCK, cafe);
				cafe->table->dinner_is_over = true;
				mutex_handler(&cafe->table->lock, UNLOCK, cafe);
				return (NULL);
			}
			else if (!cafe->all_philos[i].allowed_to_eat)
				continue ;
			else if (cafe->all_philos[i].full)
			{
				cafe->all_philos[i].allowed_to_eat = false;
				iter--;
				if (!iter)
				{
					mutex_handler(&cafe->table->lock, LOCK, cafe);
					cafe->table->dinner_is_over = true;
					mutex_handler(&cafe->table->lock, UNLOCK, cafe);
					return (NULL);
				}
			}
		}
	}
	mutex_handler(&cafe->table->lock, LOCK, cafe);
	cafe->table->dinner_is_over = true;
	mutex_handler(&cafe->table->lock, UNLOCK, cafe);
	return (NULL);
}
