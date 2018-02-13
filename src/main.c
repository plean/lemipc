#include "ipc.h"

const t_server	*server_g;

static void	usage(char const *str)
{
  printf("Usage : %s path_to_key team_number\n\n"
	 "path_to_key is a valid path that will be used by ftok\n"
	 "team_number is the team number assigned to the current player\n", str);
}

static void	handler(int signal)
{
  if (signal == SIGINT)
    {
      printf("Sigint (Ctr+C) caught\n");
      check_last_client(server_g);
      exit(0);
    }
  else if (signal == SIGQUIT)
    {
      printf("Sigquit caught\n");
      check_last_client(server_g);
      exit(0);
    }
  else if (signal == SIGTERM)
    {
      printf("Sigterm caught\n");
      check_last_client(server_g);
      exit(0);
    }
  else
    {
      printf("Unknown signal caught\n");
      check_last_client(server_g);
      exit(-1);
    }
}

static void		init_sig(void)
{
  struct sigaction	prepaSignal;

  prepaSignal.sa_handler = &handler;
  prepaSignal.sa_flags = 0;
  sigemptyset(&prepaSignal.sa_mask);

  sigaction(SIGINT, &prepaSignal, 0);
  sigaction(SIGQUIT, &prepaSignal, 0);
  sigaction(SIGTERM, &prepaSignal, 0);
}

static void	debug(t_server const *server)
{
  printf("shm: %s\n", server->shm);
  printf("key: %d\n", server->key);
  printf("shm_id: %d\n", server->shm_id);
  printf("sem_id: %d\n", server->sem_id);
}

int		main(int ac, char **av)
{
  key_t		key;

  if (ac != 3)
    {
      usage(av[0]);
      return (-1);
    }
  key = ftok(av[1], FTOK_KEY);
  server_g = check_first_client(key);
  if (!server_g)
    server_g = launch_server(key);
  debug(server_g);
  create_client(server_g, atoi(av[2]));
  init_sig();
  while (1);
  check_last_client(server_g);
  return (0);
}
