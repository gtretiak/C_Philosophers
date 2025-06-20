/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:25:24 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 17:44:00 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

int	main(void)
{
	printf("Executing times...\n");
	times(); // Getting time, sleeping and checking the time (twice)
	printf("\n\nExecuting threads...\n");
	threads(); // Thread creation, accessing the same data and race condition
	printf("\n\nExecuting mutexes...\n");
	mutexes(); //Mutexes
	printf("\n\nExecuting threads in loop...\n");
	threads_in_loop();
	printf("\n\nExecuting threads return...\n");
	threads_return();
	printf("\n\nExecuting threads take...\n");
	threads_take();
	exit(EXIT_SUCCESS);
}
