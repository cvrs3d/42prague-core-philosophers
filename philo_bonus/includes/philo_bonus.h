/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:33:25 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 15:44:13 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

typedef struct s_philo	t_philo;

typedef struct s_config
{
	time_t			start_time;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			time_to_die;
	int				must_eat_count;
	unsigned int	n_philos;
	unsigned int	philo_full_count;
	bool			stop_sim;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_philo_dead;
	sem_t			*sem_philo_full;
	sem_t			*sem_stop;
	t_philo			**philos;
	t_philo			*this_philo;
	pid_t			*pids;
	pthread_t		waiter;
	pthread_t		killer;
}	t_config;

typedef struct s_philo
{
	pthread_t		observer;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_philo_dead;
	sem_t			*sem_philo_full;
	sem_t			*sem_meal;
	char			*sem_meal_name;
	unsigned int	forks_acc;
	unsigned int	id;
	unsigned int	times_ate;
	bool			ate_enough;
	time_t			last_meal;
	t_config		*config;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

# define MAX_PHILOS	250
# define STR_MAX_PHILOS "250"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_SEM	"%s error: Could not create semaphore.\n"
# define STR_ERR_FORK	"%s error: Could not fork child.\n"

# define SEM_NAME_FORKS "/philo_global_forks"
# define SEM_NAME_WRITE "/philo_global_write"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"
# define SEM_NAME_STOP	"/philo_global_stop"
# define SEM_NAME_MEAL	"/philo_local_meal_"

# define CHILD_EXIT_ERR_PTHREAD	40
# define CHILD_EXIT_ERR_SEM		41
# define CHILD_EXIT_PHILO_FULL	42
# define CHILD_EXIT_PHILO_DEAD	43

bool		ft_has_sim_stopped(t_config *config);
bool		ft_is_valid_input(int argc, char *argv[]);
int			ft_atoi(char *str);
t_config	*ft_init_config(int argc, char *argv[], int i);
void		ft_init_philo_ipc(t_config *config, t_philo *philo);
void		ft_philosopher(t_config *config);
void		ft_grab_fork(t_philo *philo);
time_t		ft_get_time(void);
void		ft_philo_sleep(time_t sleeptime);
void		ft_sim_delay(time_t delaytime);
void		ft_write_status(t_philo *philo, bool reaper, t_status status);
void		ft_print_status(t_philo *philo, char *str);
void		ft_write_outcome(t_config *config);
void		*ft_killer(void *arg);
void		*ft_waiter(void *arg);
void		*ft_observer(void *arg);
int			ft_killall(t_config *config, int code);
char		*ft_utoa(unsigned int nb, size_t len);
char		*ft_strcat(char *dst, const char *src);
size_t		ft_strlen(const char *srt);
void		ft_unlink_g_sems(void);
bool		ft_start_threads(t_config *config);
void		*ft_free_config(t_config *config);
int			ft_sem_err_cleanup(t_config *config);
int			ft_table_cleanup(t_config *config, int code);
void		ft_child_exit(t_config *config, int code);
int			ft_msg(char *str, char *detail, int exit_no);
int			ft_erro_fail(char *str, char *details, t_config *config);
void		*ft_err_null(char *str, char *details, t_config *config);

#endif
