/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:25:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 13:00:17 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	main(void)
{
	printf("Executing times...\n");
	times(); // Getting time, sleeping and checking the time (twice)
	printf("\n__________________________________\nExecuting threads id...\n");
	threads_id();
	printf("\n__________________________________\nExecuting threads...\n");
	threads(); // Thread creation, accessing the same data and race condition
	printf("\n__________________________________\nExecuting mutexes...\n");
	mutexes(); //Mutexes
	printf("\n__________________________________\nExecuting threads in loop...\n");
	threads_in_loop();
	printf("\n__________________________________\nExecuting threads return...\n");
	threads_return();
	printf("\n__________________________________\nExecuting threads take...\n");
	threads_take();
	printf("\n__________________________________\nExecuting threads separate work...\n");
	threads_separate_work();
	exit(EXIT_SUCCESS);
}
