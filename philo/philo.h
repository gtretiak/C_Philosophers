/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:21:39 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/27 19:14:20 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>

typedef struct s_fork
{
	pthread_mutex_t	mtx;
	long	fork_id;
}	t_fork;

typedef struct s_philo
{
	int	position;
	bool	ready_to_eat; //might be a signal to the waiter ("Can I eat?")
	int	priority;//Then waiter might see at the priority and let them go ahead
	long	t_last_meal;
	t_fork	*first_fork;
	t_fork	*second_fork;
	pthread_t	philo_acting;
}	t_philo;

typedef struct s_data
{
	long	n_philos;
	long	t_eat;//akin burst time
	long	t_sleep;//paused
	long	t_die;//starvation deadline
	long	n_meals;
//	long	t_start;
	bool	the_end;
	t_fork	*all_forks;
	t_philo	*all_philos;
}	t_data;

#define ARGS "Invalid number of arguments. Should be:\n1. ./philo\n2." \
" number_of_philosophers\n3. time_to_die\n4. time_to_eat\n5. " \
"time_to_sleep\n+6. [optional] number_of_times_each_philosopher_must_eat." \
"Exiting."
#define NEGATIVE "The arguments must not be negative numbers. Exiting."
#define NON_INT "The arguments must be integer type. Exiting."
#define NO_MEALS "There is no food (0 meals). Exiting."
#define MALLOC "Malloc has failed. Exiting."
#define FORK "has taken a fork"
#define EAT "is eating"
#define SLEEP "is sleeping"
#define THINK "is thinking"
#define DIE "died"

/*
 * memset - to set a memory chunk with value
 * printf, write
 * malloc, free
 * usleep - to pause a thread execution
 * gettimeofday
 * pthread_create - to create a new thread running a routine function
 * pthread_detach - to release resources on exit (if we don't need anything from it)
 * pthread_join - to wait for the non-detached thread (e.g. to use return value)
 * pthread_mutex_init - to initialize a mutex
 * pthread_mutex_destroy - to free mutex resources (should be unlocked)
 * pthread_mutex_lock, pthread_mutex_unlock
 * */
void	add_and_check_arguments(char **argv, t_data *table);
void	init(int argc, char **argv);
void	handle_error(char *msg, int code);
void	run_simulation(t_data *table);
void	cleanup(t_data *table, int code, char *msg);
void	*dinner(void *arg);
void	*serving(void *arg);

#endif
