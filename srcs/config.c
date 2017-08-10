#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "losc.h"
#include "losc_config.h"

static char *config_path()
{
  char *cpath;

  if ((cpath = getenv("LOSC_PATH")))
    return cpath;
  if ((cpath = getenv("XDG_CONFIG_HOME")))
    return cpath;
  if ((cpath = getenv("HOME")))
    {
      asprintf(&cpath, "%s/%s/%s", getenv("HOME"), DOTCONFIG, LOSC_DIRNAME);
      return cpath;
    }
  return NULL;
}

static struct losc_config *read_config(char *cpath)
{
  lua_State *L;
  struct losc_config *config = malloc(sizeof(struct losc_config));

  L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, cpath) || lua_pcall(L, 0, 0, 0))
    LOSC_CONFIG_ERROR(lua_tostring(L, -1));

  lua_getglobal(L, "var");
  if (!lua_isnumber(L, -1))
    LOSC_CONFIG_ERROR("var must be set and a number");
  config->var = lua_tonumber(L, -1);

  lua_close(L);
  return config;
}

struct losc_config *load_config()
{
  struct losc_config *config;
  char *cpath;
  char *cfp;

  if ((cpath = config_path()) == NULL)
    return NULL;
  asprintf(&cfp, "%s/%s", cpath, "config");
  free(cpath);
  config = read_config(cfp);
  free(cfp);
  return config;
}
