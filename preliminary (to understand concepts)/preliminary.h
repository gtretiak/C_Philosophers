/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preliminary.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:38:04 by gtretiak          #+#    #+#             */
/*   Updated: 2025/06/21 13:01:11 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRELIMINARY_H
# define PRELIMINARY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

extern pthread_mutex_t	mutex;

void	times(void);
void	threads_id(void);
void	threads(void);
void	mutexes(void);
void	threads_in_loop(void);
void	threads_return(void);
void	threads_take(void);
void	threads_separate_work(void);

#endif
