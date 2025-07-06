/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:25:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/02 16:25:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	cafe;
	int	ret;

	if (argc != 5 && argc != 6)
		return (handle_error(&cafe, 1, ARGS));
	ret = add_and_check_arguments(argv, &cafe);
	if (ret)
		return (ret);
	ret = init(&cafe);
	if (ret)
		return (ret);
	ret = run_simulation(&cafe);
	if (ret)
		return (ret);
	cleanup(&cafe, 5);
	return (0);
}
/*
 *
 *create routine function with a loop until one philo died thus we need the check for it
 *determine routine and the order: think, eat, sleep, repeat 
 *implement logs
 *
 * Philos cannot communicate with each other neither they know if another philo is about to die
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
 *   That's how by using their priority we can allow the threads to take a fork*/
