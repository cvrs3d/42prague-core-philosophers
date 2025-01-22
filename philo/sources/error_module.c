/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_module.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:14:08 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 16:58:48 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_destroy_mutexes(t_config *config)
{
	unsigned int	i;

	i = 0;
	while (i < config->np_philos)
	{
		pthread_mutex_destroy(&config->fork_locks[i]);
		pthread_mutex_destroy(&config->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&config->printf_lock);
	pthread_mutex_destroy(&config->finish_lock);
}

void	*ft_free_config(t_config *config)
{
	unsigned int	i;

	if (NULL == config)
		return (NULL);
	if (NULL != config->fork_locks)
		free(config->fork_locks);
	if (NULL != config->philos)
	{
		i = 0;
		while (i < config->np_philos)
		{
			if (NULL != config->philos[i])
				free(config->philos[i]);
			i++;
		}
		free(config->philos);
	}
	free(config);
	return (NULL);
}

int	ft_msg(char *str, char *detail, int exit_status)
{
	if (NULL == detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_status);
}

int	ft_error_msg(char *str, char *details, t_config *config)
{
	if (NULL != config)
		ft_free_config(config);
	return (ft_msg(str, details, 0));
}

void	*ft_error_null(char *str, char *details, t_config *config)
{
	if (NULL != config)
		ft_free_config(config);
	ft_msg(str, details, EXIT_FAILURE);
	return (NULL);
}
