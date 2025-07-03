/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:21:39 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 19:14:30 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

# define ARGS "Error: invalid number of arguments. Exit.\n"
# define INVALID "Error: all arguments should be positive integers. Exit.\n"
# define NO_MEALS "Error: there is no food (0 meals). Exit.\n"
# define NO_PHILOS "Error: there is no philosopher. Exit.\n"
# define NO_TIME "Error: one or more timestamps is too low (min 60ms). Exit.\n"
# define MALLOC "Error: malloc has failed. Exit.\n"
# define THREAD "Error: thread has failed. Exit.\n"
# define MUTEX "Error: mutex has failed. Exit.\n"
# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DIE "died\n"

typedef struct s_fork
{
	int		index;
	pthread_mutex_t	mtx;
}	t_fork;

typedef struct s_common_data
{
	pthread_mutex_t	lock;
	pthread_mutex_t	print_lock;
	long			t_start;
	long			n_philos;
	long			t_eat;
	long			t_sleep;
	long			t_die;
	long			n_meals;
	long			n_full;
	long			running_threads;
	long			all_ready;
	long			dinner_is_over;
}	t_common_data;

typedef struct s_philo
{
	pthread_t	philo_acting;
	pthread_mutex_t	philo_lock;
	long		position;
	long		meals_eaten;
	long		t_last_meal;
	bool		allowed_to_eat; //or ready to eat as a signal to the waiter?
				//priority for the waiter? OR for immediate exit if not allowed 
	bool		rip;
	bool		full;
	t_common_data		*table;
	t_fork			*first_fork;
	t_fork			*second_fork;
}	t_philo;

typedef struct s_data
{
	t_common_data	*table;
	t_fork			*all_forks;
	t_philo			*all_philos;
	pthread_t		waiter;
}	t_data;

typedef enum e_code
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN
}	t_code;
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

int		init(t_data *cafe);
int		add_and_check_arguments(char **argv, t_data *cafe);
int		eating_phase(t_philo *philo);

void	*dinner(void *arg);
void	*serving(void *arg);
int		run_simulation(t_data *cafe);
void	*run_alone(void *arg);

int		handle_error(t_data *cafe, int code, char *msg);
void	cleanup(t_data *cafe, int code);
int		mutex_handler(pthread_mutex_t *mutex, t_code code);

int		wait_others(t_philo *philo);
int		all_running(t_common_data *table);
long	get_time(void);
int		precise_usleep(long sleeping_time, t_common_data *table);
int		printing_status(t_philo *philo, char *msg);

long	get_long(pthread_mutex_t *mutex, long *value);
int		set_long(pthread_mutex_t *mutex, long *var, long value);

#endif
