/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc_module.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:54:34 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:58:04 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->config->n_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->config->n_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->config->n_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

static bool	philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

void	ft_init_philo_ipc(t_config *config, t_philo *philo)
{
	if (config->n_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!philo_open_global_semaphores(philo))
		ft_child_exit(config, CHILD_EXIT_ERR_SEM);
	if (!philo_open_local_semaphores(philo))
		ft_child_exit(config, CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->observer, NULL,
			&ft_observer, config) != 0)
		ft_child_exit(config, CHILD_EXIT_ERR_PTHREAD);
	return ;
}
