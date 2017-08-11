#include <stdlib.h>

#include "losc.h"
#include "losc_config.h"

int main(int ac, char **av)
{
  UNUSED(ac);
  UNUSED(av);
  struct losc_config *config = losc_load_config();
  free(config);
  return EXIT_SUCCESS;
}