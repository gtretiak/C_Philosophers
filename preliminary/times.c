/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:52:46 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/20 15:52:52 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

void	times(void)
{
	struct timeval	current_time;
	gettimeofday(&current_time, NULL);
	printf("Seconds: %ld\nMicroseconds: %ld\n",
		current_time.tv_sec, current_time.tv_usec);
	printf("Sleeping for 500000 microseconds...\n");
	usleep(500000);
	printf("Done sleeping.\n");
	gettimeofday(&current_time, NULL);
	printf("Seconds: %ld\nMicroseconds: %ld\n\n\n",
		current_time.tv_sec, current_time.tv_usec);
}
