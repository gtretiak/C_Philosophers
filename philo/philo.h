/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:21:39 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/13 12:46:20 by gtretiak         ###   ########.fr       */
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
# define TIME "Error: gettimeofday has failed. Exit.\n"
# define MUTEX "Error: mutex has failed. Exit.\n"
# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DIE "died\n"

typedef struct s_fork
{
	pthread_mutex_t	lock;
	long			taken;
}	t_fork;

typedef struct s_table
{
	pthread_mutex_t	lock;
	pthread_mutex_t	print_lock;
	long			t_start;
	long			n_philos;
	long			n_philos_reserved;
	long			t_eat;
	long			t_sleep;
	long			t_die;
	long			n_meals;
	long			n_full;
	long			running_threads;
	long			all_ready;
	long			dinner_is_over;
}	t_table;

typedef struct s_philo
{
	pthread_mutex_t	lock;
	pthread_t		th;
	long			pos;
	long			meals_eaten;
	long			t_last_meal;
//	bool		allowed_to_eat; //or ready to eat as a signal to the waiter?
//				//priority for the waiter? OR for immediate exit if not allowed 
	long			rip;
	long			full;
	long			counted_full;
	t_table			*table;
	t_fork			*fork1;
	t_fork			*fork2;
}	t_philo;

typedef struct s_data
{
	t_table			*table;
	t_fork			*forks;
	t_philo			*philos;
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

typedef enum e_time
{
	US,
	MS
}	t_time;

int		add_and_check_arguments(char **argv, t_data *cafe);
int		init(t_data *cafe);

int		run_simulation(t_data *cafe);

void	*run_alone(void *arg);

void	*dinner(void *arg);
int		thinking_phase(t_philo *philo, bool preset);
int		think_about(t_philo *philo);
int		eating_phase(t_philo *philo);

void	*serving(void *arg);
int		is_philo_dead(t_philo *philo);
int		is_everyone_full(t_philo *philo, long iter);

int		cleanup(int code, int num, char *msg, t_data *cafe);
void	*write_error(char *msg, t_table *table);
int		printing_status(t_philo *philo, char *msg);

int		wait_others(t_philo *philo);
int		all_running(t_table *table);
int		ft_strncmp(char *s1, char *s2, int n);
int		handle_mtx(pthread_mutex_t *mtx, t_code code, t_table *table);

long	get_time(t_time time, t_table *table);
int		sleeping(long sleeping_time, long start, t_table *table);

long	increase_long(pthread_mutex_t *mtx, long *value, t_table *table);
long	get_long(pthread_mutex_t *mtx, long *value, t_table *table);
int		set_long(pthread_mutex_t *mtx, long *var, long value, t_table *table);
int		set_bool(pthread_mutex_t *mtx, bool *var, bool value, t_table *table);

#endif
