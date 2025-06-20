/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:25:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 19:11:47 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	if (argc < 5)
		handle_error(ARGS, 2);
	else
	{
		for (int i = 0; i < argc; i++)
			printf("%s\n", argv[i]);
	}
	exit(EXIT_SUCCESS);
}

/*
 *store the user input to a struct
 *create threads aka philos
 *create routine function with a loop until one philo died thus we need the check for it
 *determine routine and the order: think, eat, sleep, repeat 
 *implement logs
 *lock/unlock the same value
 *bonus: semaphors and processes
 */
