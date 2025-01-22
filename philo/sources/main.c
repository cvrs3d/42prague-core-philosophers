/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:04:44 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/21 17:35:21 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_stop_simulation(t_config *config)
{
	unsigned int	i;

	i = 0;
	while (i < config->np_philos)
	{
		pthread_join(config->philos[i]->pthread, NULL);
		i++;
	}
	if (config->np_philos > 1)
		pthread_join(config->arbitrator, NULL);
	if (DEBUG_FORAMTING == true && config->must_eat_count != -1)
		ft_write_outcome(config);
	ft_destroy_mutexes(config);
	ft_free_config(config);
}

static bool	ft_start_simulation(t_config *config)
{
	unsigned int	i;

	config->init_time = ft_get_time() + (config->np_philos * 20);
	i = 0;
	while (i < config->np_philos)
	{
		if (pthread_create(&config->philos[i]->pthread, NULL,
				ft_routine, config->philos[i]) != 0)
			return (ft_error_msg(STR_ERR_THREAD, NULL, config));
		i++;
	}
	if (config->np_philos > 1)
	{
		if (pthread_create(&config->arbitrator, NULL,
				ft_arbitrator, config) != 0)
			return (ft_error_msg(STR_ERR_THREAD, NULL, config));
	}
	return (true);
}

int	main(int argc, char *argv[])
{
	t_config	*config;

	config = NULL;
	if (argc - 1 < 4 || argc - 1 > 5)
		return (ft_msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!ft_is_valid_input(argc, argv))
		return (EXIT_FAILURE);
	config = ft_init_config(argc, argv, 1);
	if (NULL == config)
		return (EXIT_FAILURE);
	if (!ft_start_simulation(config))
		return (EXIT_FAILURE);
	ft_stop_simulation(config);
	return (EXIT_SUCCESS);
}
