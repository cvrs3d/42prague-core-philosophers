/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 11:04:47 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 16:49:03 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	ft_has_sim_stopped(t_config *config)
{
	bool	retval;

	sem_wait(config->sem_stop);
	retval = config->stop_sim;
	sem_post(config->sem_stop);
	return (retval);
}

static bool	ft_start_simulation(t_config *config)
{
	unsigned int	i;
	pid_t			pid;

	config->start_time = ft_get_time() + (config->n_philos * 20);
	i = -1;
	while (++i < config->n_philos)
	{
		pid = fork();
		if (-1 == pid)
			return (ft_erro_fail(STR_ERR_FORK, NULL, config));
		else if (pid > 0)
			config->pids[i] = pid;
		else if (pid == 0)
		{
			config->this_philo = config->philos[i];
			printf("Philosopher is up");
			ft_philosopher(config);
		}
	}
	if (false == ft_start_threads(config))
		return (false);
	return (true);
}

static int	ft_get_child(t_config *config, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (ft_killall(config, 1));
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (ft_killall(config, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				config->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

static int	ft_stop_simulation(t_config *config)
{
	unsigned int	i;
	int				exit_code;

	ft_sim_delay(config->start_time);
	while (ft_has_sim_stopped(config) == false)
	{
		i = 0;
		while (i < config->n_philos)
		{
			exit_code = ft_get_child(config, &config->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(config->sem_stop);
				config->stop_sim = true;
				sem_post(config->sem_philo_full);
				sem_post(config->sem_philo_dead);
				sem_post(config->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_config	*config;

	config = NULL;
	if (4 > argc - 1 || 5 < argc - 1)
		return (ft_msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!ft_is_valid_input(argc, argv))
		return (EXIT_FAILURE);
	config = ft_init_config(argc, argv, 1);
	if (NULL == config)
		return (EXIT_FAILURE);
	if (!ft_start_simulation(config))
		return (EXIT_FAILURE);
	if (-1 == ft_stop_simulation(config))
		return (ft_table_cleanup(config, EXIT_FAILURE));
	return (ft_table_cleanup(config, EXIT_SUCCESS));
}
