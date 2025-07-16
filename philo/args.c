/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtretiak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:59:07 by gtretiak          #+#    #+#             */
/*   Updated: 2025/07/10 18:59:08 by gtretiak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ft_isspace(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

static bool	ft_isdigit(char c)
{
	return (c >= 48 && c <= 57);
}

static long	spec_atol(char *s)
{
	long	res;
	int		i;

	res = 0; // we start with res = 0
	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] && (s[i] == '-' || s[i] == '+'))
	{
		if (s[i] == '-')
			return (-2); // we do not accept negative values
		i++;
	}
	if (s[i] && !ft_isdigit(s[i]))
		return (-2);
	while (s[i] && ft_isdigit(s[i]))
	{
		res = res * 10 + (s[i] - '0');
		if (res > INT_MAX)
			return (-2);
		i++;
	}
	return (res);
}

static int	init_data(char **argv, t_data *cafe)
{
	cafe->table = malloc(sizeof(t_table));
	if (cafe->table == NULL)
		return (cleanup(12, 0, MALLOC, cafe)); // Zero means we have nothing to free
	cafe->table->n_philos = spec_atol(argv[1]);
	cafe->table->t_die = spec_atol(argv[2]);
	cafe->table->t_eat = spec_atol(argv[3]);
	cafe->table->t_sleep = spec_atol(argv[4]);
	if (argv[5])
		cafe->table->n_meals = spec_atol(argv[5]);
	else
		cafe->table->n_meals = -1; // -1 is a flag telling us we do not have the optional argument
	return (0);
}

int	add_and_check_arguments(char **argv, t_data *cafe)
{
	int	ret;

	ret = init_data(argv, cafe);
	if (ret)
		return (ret);
	if (cafe->table->n_meals == 0)
		return (cleanup(22, 1, NO_MEALS, cafe)); // 1 means that only table should be freed
	else if (cafe->table->n_meals == -2)
		return (cleanup(22, 1, INVALID, cafe));
	if (cafe->table->n_philos == 0)
		return (cleanup(22, 1, NO_PHILOS, cafe));
	else if (cafe->table->n_philos < 0)
		return (cleanup(22, 1, INVALID, cafe));
	if (cafe->table->t_die < 0
		|| cafe->table->t_eat < 0
		|| cafe->table->t_sleep < 0)
		return (cleanup(22, 1, INVALID, cafe));
	else if (cafe->table->t_die < 60
		|| cafe->table->t_eat < 60
		|| cafe->table->t_sleep < 60)
		return (cleanup(22, 1, NO_TIME, cafe));
	return (0);
}
/*
* Under system load, timers (used by usleep and gettimeofday) become less precise, 
* and threads may be scheduled with delays that affect execution timing and synchronization. 
* The smaller the timestamp values, the greater the discrepancy between expected and actual timing, 
* and the more CPU-intensive the polling becomes. 
* 60ms is an empirically chosen safe threshold that ensures reliable and consistent behavior across different systems.
*/
