/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_module.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:44:45 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 14:48:17 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_unlink_g_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}

bool	ft_start_threads(t_config *config)
{
	if (pthread_create(&config->waiter, NULL,
			&ft_waiter, config) != 0)
		return (ft_erro_fail(STR_ERR_THREAD, NULL, config));
	if (pthread_create(&config->killer, NULL,
			&ft_killer, config) != 0)
		return (ft_erro_fail(STR_ERR_THREAD, NULL, config));
	return (true);
}
