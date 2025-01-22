/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_module.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:05:34 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:46:43 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_free_config(t_config *config)
{
	unsigned int	i;

	if (!config)
		return (NULL);
	if (config->philos != NULL)
	{
		i = 0;
		while (i < config->n_philos)
		{
			if (config->philos[i] != NULL)
			{
				if (config->philos[i]->sem_meal_name)
					free(config->philos[i]->sem_meal_name);
				free(config->philos[i]);
			}
			i++;
		}
		free(config->philos);
	}
	if (config->pids)
		free(config->pids);
	free(config);
	return (NULL);
}

int	ft_sem_err_cleanup(t_config *config)
{
	sem_close(config->sem_forks);
	sem_close(config->sem_write);
	sem_close(config->sem_philo_full);
	sem_close(config->sem_philo_dead);
	sem_close(config->sem_stop);
	ft_unlink_g_sems();
	return (ft_erro_fail(STR_ERR_SEM, NULL, config));
}

int	ft_table_cleanup(t_config *config, int code)
{
	if (config != NULL)
	{
		pthread_join(config->killer, NULL);
		pthread_join(config->waiter, NULL);
		sem_close(config->sem_forks);
		sem_close(config->sem_write);
		sem_close(config->sem_philo_full);
		sem_close(config->sem_philo_dead);
		sem_close(config->sem_stop);
		ft_unlink_g_sems();
		ft_free_config(config);
	}
	return (code);
}
