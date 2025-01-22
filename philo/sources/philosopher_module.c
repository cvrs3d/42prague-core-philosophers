/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_module.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:28:00 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 17:34:54 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eat_sleep_routine(t_thread *philo)
{
	pthread_mutex_lock(&philo->config->fork_locks[philo->fork[0]]);
	ft_write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->config->fork_locks[philo->fork[1]]);
	ft_write_status(philo, false, GOT_FORK_2);
	ft_write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = ft_get_time();
	pthread_mutex_unlock(&philo->meal_time_lock);
	ft_thread_sleep(philo->config, philo->config->time_to_eat);
	if (ft_has_sim_stopped(philo->config) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	ft_write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->config->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->config->fork_locks[philo->fork[0]]);
	ft_thread_sleep(philo->config, philo->config->time_to_sleep);
}

static void	ft_think_routine(t_thread *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->config->time_to_die
			- (ft_get_time() - philo->last_meal)
			- philo->config->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		ft_write_status(philo, false, THINKING);
	ft_thread_sleep(philo->config, time_to_think);
}

static void	*ft_lone_thread(t_thread *philo)
{
	pthread_mutex_lock(&philo->config->fork_locks[philo->fork[0]]);
	ft_write_status(philo, false, GOT_FORK_1);
	ft_thread_sleep(philo->config, philo->config->time_to_die);
	ft_write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->config->fork_locks[philo->fork[0]]);
	return (NULL);
}

void	*ft_routine(void *arg)
{
	t_thread	*philo;

	philo = (t_thread *)arg;
	if (philo->config->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->config->printf_lock);
	philo->last_meal = philo->config->init_time;
	pthread_mutex_unlock(&philo->config->printf_lock);
	ft_sim_start_delay(philo->config->init_time);
	if (philo->config->time_to_die == 0)
		return (NULL);
	if (philo->config->np_philos == 1)
		return (ft_lone_thread(philo));
	else if (philo->id % 2)
		ft_think_routine(philo, true);
	while (ft_has_sim_stopped(philo->config) == false)
	{
		ft_eat_sleep_routine(philo);
		ft_think_routine(philo, false);
	}
	return (NULL);
}
