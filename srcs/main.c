#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *strdup(char *s);

#include <getopt.h>

#include "losc.h"

extern char *program_invocation_name;

static struct option long_options[] =
  {
    { "config", required_argument, NULL, 'c' },
    { "atr", required_argument, NULL, 'a' },
    { "locker", required_argument, NULL, 'l' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };

static void help(bool v)
{
  printf("usage: %s [-h] [-c] [-a] [-l]\n", program_invocation_name);
  if (!v)
    return;
  printf("  -c, --config CONFIG_PATH: specifies the configuration file's path\n");
  printf("  -a, --atr ATR: specifies a valid ATR for this session\n");
  printf("  -l, --locker SCREEN_LOCKER: specifies a screen locker to kill\n");
  printf("  -h, --help: shows this help\n");
  printf("losc v"LOSC_VERSION" - "LOSC_AUTHORS"\n");
}

static void init_daemon()
{
  openlog(NULL, LOG_CONS, LOG_DAEMON);
  atexit(&losc_release);
}

int main(int argc, char **argv)
{
  int c;
  int idx;
  char *config_path = NULL;

  struct losc_config *config = malloc(sizeof(struct losc_config));
  config->atrs = NULL;
  config->locker = NULL;

  while ((c = getopt_long(argc, argv, "c:a:l:h", long_options, &idx)) != EOF)
    {
      switch (c)
        {
          case 'a':
            config->atrs = malloc(sizeof(char *) * 2);
            config->atrs[0] = strdup(optarg);
            config->atrs[1] = NULL;
            break;

          case 'c':
            config_path = strdup(optarg);
            break;

          case 'l':
            config->locker = strdup(optarg);
            break;

          case 'h':
            help(true);
            return 1;
            break;

          case '?':
          default:
            help(false);
            return 1;
            break;
        }
    }
  
  init_daemon();
  if (!losc_load_config(config_path, config))
    return 1;
  return losc_daemon_loop(config);
}
