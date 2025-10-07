/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:20:33 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:20:33 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_threads(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->num_philos)
	{
		if (pthread_create(&r->philos[i].thread, NULL, philo_routine
				, &r->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&r->monitor_thread, NULL, monitor_routine, r))
		return (1);
	pthread_mutex_lock(&r->state_lock);
	r->start_time = now_ms();
	i = 0;
	while (i < r->num_philos)
	{
		pthread_mutex_lock(&r->philos[i].meal_lock);
		r->philos[i].last_meal = r->start_time;
		pthread_mutex_unlock(&r->philos[i].meal_lock);
		i++;
	}
	r->start_sim = 1;
	pthread_mutex_unlock(&r->state_lock);
	usleep(100);
	return (0);
}

static void	join_threads(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->num_philos)
	{
		pthread_join(r->philos[i].thread, NULL);
		i++;
	}
	pthread_join(r->monitor_thread, NULL);
}

void	waiting(t_philo *p)
{
	while (1)
	{
		pthread_mutex_lock(&p->rules->state_lock);
		if (p->rules->start_sim)
		{
			pthread_mutex_unlock(&p->rules->state_lock);
			break ;
		}
		pthread_mutex_unlock(&p->rules->state_lock);
		usleep(50);
	}
}

// void	waiting(t_philo *p)
// {
// 	while (1)
// 	{
// 		pthread_mutex_lock(&p->rules->state_lock);
// 		if (p->rules->start_sim)
// 		{
// 			pthread_mutex_unlock(&p->rules->state_lock);
// 			break ;
// 		}
// 		pthread_mutex_unlock(&p->rules->state_lock);
// 		usleep(50);
// 	}
// }

int	main(int ac, char **av)
{
	t_rules	r;

	if (parse_and_init(ac, av, &r))
		return (1);
	if (start_threads(&r))
	{
		destroy_and_free(&r);
		return (1);
	}
	join_threads(&r);
	destroy_and_free(&r);
	return (0);
}
