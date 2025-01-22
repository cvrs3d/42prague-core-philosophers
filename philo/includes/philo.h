/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yustinov <yustinov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:04:56 by yustinov          #+#    #+#             */
/*   Updated: 2025/01/22 13:42:04 by yustinov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

/*\
 *	Macros
\*/

# ifndef DEBUG_FORMATING
#  define DEBUG_FORAMTING 0
# endif
# define MAX_THREADS 250
# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define STR_MAX_THREADS "250"
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
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

/*\
 *	Structures
\*/

typedef struct s_config	t_config;
typedef struct s_thread	t_thread;

typedef struct s_config
{
	time_t			init_time;
	unsigned int	np_philos;
	pthread_t		arbitrator;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			finish;
	pthread_mutex_t	finish_lock;
	pthread_mutex_t	printf_lock;
	pthread_mutex_t	*fork_locks;
	t_thread		**philos;
}	t_config;

typedef struct s_thread
{
	pthread_t		pthread;
	unsigned int	id;
	unsigned int	times_ate;
	unsigned int	fork[2];
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_config		*config;
}	t_thread;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

/*\
 *	Modules
\*/

/* Error module */
void		*ft_free_config(t_config *config);
int			ft_msg(char *str, char *detail, int exit_status);
int			ft_error_msg(char *str, char *details, t_config *config);
void		*ft_error_null(char *str, char *details, t_config *config);
void		ft_destroy_mutexes(t_config *config);

/* Init module */
t_config	*ft_init_config(int argc, char *argv[], int i);

/* Output module */
void		ft_write_status(t_thread *philo, bool report, t_status status);
void		ft_write_outcome(t_config *config);

/* Time module */
void		ft_sim_start_delay(time_t	start_time);
void		ft_thread_sleep(t_config *config, time_t sleep_time);
time_t		ft_get_time(void);

/* Parse module */
bool		ft_is_valid_input(int argc, char *argv[]);
int			ft_atoi(char *str);

/* Philosopher module  */
void		*ft_routine(void *arg);

/* Arbitrator module */
void		*ft_arbitrator(void *arg);
bool		ft_has_sim_stopped(t_config *config);

#endif
