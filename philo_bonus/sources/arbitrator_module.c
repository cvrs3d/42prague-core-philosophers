/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitrator_module.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:28:16 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:47:54 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_killall(t_config *config, int code)
{
	unsigned int	i;

	i = 0;
	while (i < config->n_philos)
	{
		kill(config->pids[i], SIGKILL);
		i++;
	}
	return (code);
}

void	*ft_waiter(void *arg)
{
	t_config	*config;

	config = (t_config *)arg;
	if (config->must_eat_count < 0 || config->time_to_die == 0
		|| config->n_philos == 1)
		return (NULL);
	ft_sim_delay(config->start_time);
	while (config->philo_full_count < config->n_philos)
	{
		if (ft_has_sim_stopped(config) == true)
			return (NULL);
		sem_wait(config->sem_philo_full);
		if (ft_has_sim_stopped(config) == false)
			config->philo_full_count += 1;
		else
			return (NULL);
	}
	sem_wait(config->sem_stop);
	config->stop_sim = true;
	ft_killall(config, EXIT_SUCCESS);
	sem_post(config->sem_philo_dead);
	sem_post(config->sem_stop);
	return (NULL);
}

void	*ft_killer(void *arg)
{
	t_config	*config;

	config = (t_config *)arg;
	if (config->n_philos == 1)
		return (NULL);
	ft_sim_delay(config->start_time);
	if (ft_has_sim_stopped(config) == true)
		return (NULL);
	sem_wait(config->sem_philo_dead);
	if (ft_has_sim_stopped(config) == true)
		return (NULL);
	sem_wait(config->sem_stop);
	config->stop_sim = true;
	ft_killall(config, EXIT_SUCCESS);
	sem_post(config->sem_philo_full);
	sem_post(config->sem_stop);
	return (NULL);
}

static bool	end_condition_reached(t_config *config, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (ft_get_time() - philo->last_meal >= config->time_to_die)
	{
		ft_write_status(philo, true, DIED);
		sem_post(config->this_philo->sem_philo_dead);
		sem_post(philo->sem_meal);
		return (true);
	}
	if (config->must_eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= (unsigned int)config->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

void	*ft_observer(void *arg)
{
	t_config			*config;

	config = (t_config *)arg;
	if (config->must_eat_count == 0)
		return (NULL);
	sem_wait(config->this_philo->sem_philo_dead);
	sem_wait(config->this_philo->sem_philo_full);
	ft_sim_delay(config->start_time);
	while (!end_condition_reached(config, config->this_philo))
	{
		usleep(1000);
		continue ;
	}
	return (NULL);
}
