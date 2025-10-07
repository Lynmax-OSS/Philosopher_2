/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keteo <keteo@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:22:23 by keteo             #+#    #+#             */
/*   Updated: 2025/09/02 13:22:23 by keteo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_and_free(t_rules *r)
{
	int	i;

	if (!r)
		return ;
	if (r->forks)
	{
		i = 0;
		while (i < r->num_philos)
		{
			pthread_mutex_destroy(&r->philos[i].meal_lock);
			pthread_mutex_destroy(&r->forks[i]);
			i++;
		}
		free(r->forks);
	}
	pthread_mutex_destroy(&r->print_lock);
	pthread_mutex_destroy(&r->state_lock);
	free(r->philos);
}
