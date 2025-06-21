/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:21:39 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 17:53:14 by gtretiak         ###   ########.fr       */
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

typedef struct s_philo
{
	int	n_philos;
	int	position;
	bool	ready_to_eat; //might be a signal to the waiter ("Can I eat?")
	int	priority;//Then waiter might see at the priority and let them go ahead
	unsigned int	t_eat;//akin burst time
	unsigned int	t_sleep;//paused
	unsigned int	t_die;//starvation deadline
	int	n_meals;
	pthread_t	philo_acting;
}	t_philo;

#define ARGS "Invalid number of arguments. Should be:\n1. ./philo\n2." \
" number_of_philosophers\n3. time_to_die\n4. time_to_eat\n5. " \
"time_to_sleep\n+6. [optional] number_of_times_each_philosopher_must_eat."
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
void	init(int argc, char **argv);
void	handle_error(char *msg, int code);
void	*dinner(void *arg);

#endif
