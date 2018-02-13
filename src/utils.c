#include "ipc.h"

extern const t_server	*server_g;

void	die(char const *str)
{
  check_last_client(server_g);
  perror(str);
  exit(-1);
}
