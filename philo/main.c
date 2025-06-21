/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:25:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 17:54:25 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{

	if (argc < 5)
		handle_error(ARGS, 2);
	else // meat
	{
		init(argc, argv);//TODO
		pthread_mutex_t	mutex[argv[1]];//TODO should be int, not a char*
	for (int i = 0; i < argv[1]; i++)
		pthread_mutex_init(&mutex[i], NULL);
	for (int i = 0; i < argv[1]; i++)
		pthread_mutex_destroy(&mutex[i], NULL);
	}
	exit(EXIT_SUCCESS);
}

/*
 *store the user input to a struct
 *create threads aka philos
 *create routine function with a loop until one philo died thus we need the check for it
 *determine routine and the order: think, eat, sleep, repeat 
 *implement logs
 *
 * we can let different philos go ahead in an infinite loop, then throw some value back, take them in a loop and use in the consecutive calls...
 *
 *lock/unlock the same value, but not only this (we need to make sure another fork is available).
 *
 *if mutex is locked other threads should either:
 - constantly busy wait - good if blocking time is short;
 - wait in an atomic mode (spinlock) - good if blocking time is long
 - poll for updates from time to time and sleep [PREFERED OPTION] for a meal time?
 *
 * Deadlock - when someone waits for resources (forks) to release
 * Conditions:
 * - the resource (a fork) is locked
 * - the resource (one fork) is taken, but another resource (another fork) is locked
 *Deadlock might be avoided, prevented, detected and mitigated or ignored (with data overwritten)

 *   Starvation - when the process with lower priority is long waiting (A philo dies)
 *   When a philo is ready, it should step in a queue?
 *   That's how by using their priority we can allow the threads to take a fork
 *bonus: semaphors and processes

https://www.youtube.com/watch?v=OIKr2ll2Nd8&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=9
https://www.youtube.com/watch?v=zOpzGHwJ3MU
https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/
https://linuxhint.com/pthread-join-multiple-threads-example/?source=post_page-----7157cc05315---------------------------------------
https://www.cs.cornell.edu/courses/cs4411/2020sp/schedule/slides/03-Interrupts-Quanta.pdf
*/
