#pragma once
#ifndef LOSC_CONFIG_H_
# define LOSC_CONFIG_H_

# include <error.h>

# define DOTCONFIG ".config"
# define LOSC_CONFIG_ERROR(...) \
  error(1, 0, "Error while reading configuration:\n\t%s.", __VA_ARGS__)

struct losc_config
{
  int var;
};

struct losc_config *load_config();

#endif