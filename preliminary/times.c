/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:52:46 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 17:01:14 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "preliminary.h"

void	times(void)
{
	struct timeval	current_time, pasttime;
	gettimeofday(&current_time, NULL);
	printf("Seconds: %ld\nMicroseconds: %ld\n",
		current_time.tv_sec, current_time.tv_usec);
	printf("Sleeping for 500 000 microseconds (0.5 sec)...\n");
	usleep(500000);
	gettimeofday(&pasttime, NULL);
	printf("Actual usleep time:\nSeconds:%ld\nMicroseconds: %ld\n\n",
		pasttime.tv_sec - current_time.tv_sec, 
		pasttime.tv_usec - current_time.tv_usec);
	printf("\n\n_____________________________________________\n"
		"Human readable time:\n");
	char	buf[64];
	struct tm	*info;
	info = localtime(&current_time.tv_sec);
	strftime(buf, sizeof(buf), "Today is %A, %B %d.\n", info);
	printf("Version 1: %s", buf);
	strftime(buf, sizeof(buf), "The time is %I:%M %p.\n", info);
	printf("Version 2: %s", buf);
	printf("Version 3: %s\n\n", asctime(info));
}
