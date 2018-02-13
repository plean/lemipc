#include "ipc.h"

t_server	*check_first_client(key_t key)
{
  t_server	*server;

  server = malloc(sizeof(*server));
  if (server == NULL)
    die("malloc");
  server->shm_id = shmget(key, SERVER_INFO, SHM_R | SHM_W);
  if (server->shm_id == -1)
    {
      printf("server needed\n");
      return (NULL);
    }
  server->shm = shmat(server->shm_id, NULL, 0);
  if (server->shm == (void *)-1)
    die("shmid");
  server->sem_id = semget(key, PLAYER_NB_SEMS, SHM_R | SHM_W);
  if (server->sem_id == -1)
    die("semget");
  server->key = key;
  return (server);
}

t_server	*launch_server(key_t key)
{
  t_server	*server;

  server = malloc(sizeof(*server));
  if (server == NULL)
    die("malloc");
  server->shm_id = shmget(key, SERVER_INFO, IPC_CREAT | SHM_R | SHM_W);
  if (server->shm_id == -1)
    die("shmget");
  server->shm = shmat(server->shm_id, NULL, 0);
  if (server->shm == (void *)-1)
    die("shmid");
  server->sem_id = semget(key, PLAYER_NB_SEMS, IPC_CREAT | SHM_R | SHM_W);
  if (server->sem_id == -1)
    die("semget");
  server->key = key;
  if (semctl(server->sem_id, PLAYER_NUMBER, SETVAL, 0) == -1)
    die("semctl");
  printf("server created\n");
  return (server);
}

void	create_client(t_server const *server, int team_id)
{
  int	nb_player;

  (void)team_id;
  nb_player = semctl(server->sem_id, PLAYER_NUMBER, GETVAL);
  if (nb_player == -1)
    die("semctl");
  nb_player += 1;
  if (semctl(server->sem_id, PLAYER_NUMBER, SETVAL, nb_player) == -1)
    die("semctl");
  printf("client n%d created\n", nb_player);
}

void	check_last_client(t_server const *server)
{
  int	nb_player;

  nb_player = semctl(server->sem_id, PLAYER_NUMBER, GETVAL);
  if (nb_player <= 1)
    {
      printf("server deleted\n");
      shmctl(server->shm_id, IPC_RMID, NULL);
    }
  if (semctl(server->sem_id, PLAYER_NUMBER, SETVAL, nb_player - 1) == -1)
    {
      perror("semctl");
      exit(-1);
    }
}
