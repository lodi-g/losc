#pragma once
#ifndef LOSC_CONFIG_H_
# define LOSC_CONFIG_H_

# include <error.h>

# define LOSC_DOTCONFIG ".config"
# define LOSC_CONFIG_ERROR(...) \
  error(1, 0, "Error while reading configuration:\n\t%s.", __VA_ARGS__)

struct losc_config
{
  bool b;
  int var;
  char *str;
};

struct losc_config *losc_load_config();

#endif