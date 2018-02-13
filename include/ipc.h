#ifndef IPC_H_
# define IPC_H_

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <sys/ipc.h>
# include <sys/types.h>
# include <sys/shm.h>
# include <sys/sem.h>

# define FTOK_KEY 0
# define SERVER_INFO 1

typedef enum	e_player_info
{
    PLAYER_NUMBER,
    PLAYER_NB_SEMS,
}		t_player_info;

typedef struct	s_server
{
  char		*shm;
  key_t		key;
  int		shm_id;
  int		sem_id;
}		t_server;

t_server	*check_first_client(key_t key);
t_server	*launch_server(key_t key);
void		create_client(t_server const *server, int team_id);
void		check_last_client(t_server const *server);
void		die(char const *str);

#endif /* !IPC_H_ */
