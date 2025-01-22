/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitrator_module.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:46:15 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 17:35:07 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_set_sim_stop_flag(t_config *config, bool state)
{
	pthread_mutex_lock(&config->finish_lock);
	config->finish = state;
	pthread_mutex_unlock(&config->finish_lock);
}

bool	ft_has_sim_stopped(t_config *config)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&config->finish_lock);
	if (config->finish == true)
		r = true;
	pthread_mutex_unlock(&config->finish_lock);
	return (r);
}

static bool	ft_kill_philo(t_thread *philo)
{
	time_t	time;

	time = ft_get_time();
	if ((time - philo->last_meal) >= philo->config->time_to_die)
	{
		ft_set_sim_stop_flag(philo->config, true);
		ft_write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	ft_end_cond_reached(t_config *config)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < config->np_philos)
	{
		pthread_mutex_lock(&config->philos[i]->meal_time_lock);
		if (ft_kill_philo(config->philos[i]))
			return (true);
		if (config->must_eat_count != -1)
			if (config->philos[i]->times_ate
				< (unsigned int)config->must_eat_count)
				all_ate_enough = false;
		pthread_mutex_unlock(&config->philos[i]->meal_time_lock);
		i++;
	}
	if (config->must_eat_count != -1 && all_ate_enough == true)
	{
		ft_set_sim_stop_flag(config, true);
		return (true);
	}
	return (false);
}

void	*ft_arbitrator(void *arg)
{
	t_config	*config;

	config = (t_config *)arg;
	if (config->must_eat_count == 0)
		return (NULL);
	ft_set_sim_stop_flag(config, false);
	ft_sim_start_delay(config->init_time);
	while (true)
	{
		if (ft_end_cond_reached(config) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
