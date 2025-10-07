/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:21:46 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:21:46 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_rules *r)
{
	int			i;
	long long	now;
	long long	elapsed_time;

	i = 0;
	while (i < r->num_philos)
	{
		pthread_mutex_lock(&r->philos[i].meal_lock);
		now = now_ms();
		elapsed_time = now - r->philos[i].last_meal;
		pthread_mutex_unlock(&r->philos[i].meal_lock);
		pthread_mutex_lock(&r->state_lock);
		if (!r->someone_died && elapsed_time >= r->time_to_die)
		{
			r->someone_died = 1;
			pthread_mutex_unlock(&r->state_lock);
			safe_print(r, r->philos[i].id, "died");
			return (1);
		}
		pthread_mutex_unlock(&r->state_lock);
		i++;
	}
	return (0);
}

static int	check_full(t_rules *r)
{
	int	i;
	int	full;

	if (r->must_eat <= 0)
		return (0);
	i = 0;
	full = 1;
	pthread_mutex_lock(&r->state_lock);
	while (i < r->num_philos)
	{
		if (r->philos[i].meals_eaten < r->must_eat)
			full = 0;
		i++;
	}
	if (full)
		r->all_satiated = 1;
	pthread_mutex_unlock(&r->state_lock);
	return (full);
}

void	*monitor_routine(void *arg)
{
	t_rules	*r;

	r = (t_rules *)arg;
	while (1)
	{
		if (check_death(r))
			break ;
		if (check_full(r))
			break ;
		usleep(1);
	}
	return (NULL);
}
