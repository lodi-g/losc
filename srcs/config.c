#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "losc.h"

/*
** Losc configuration
**
** Losc will search its configuration with the following priority
**  - -c/--config command line option
**  - $LOSC_PATH/config
**  - $XDG_CONFIG_HOME/losc/config
**  - $HOME/.config/losc/config
**
** Once the path has been defined, it'll try to read it using lua interpreter,
** allowing the user to do conditional configuration (and making losc's code easier to read & maintain)
**
** It'll retrieve two global variables: 
**  - locker, type=string, the screen locker which will be killed when the card is inserted
**    (can be specified with the -l/--locker, in that case losc won't read the config)
**  - atrs, type=array of string, the ATRs that will trigger the unlock action
**    (ca be specified with -a/--atr)
** 
** Once the configuration has been retrieved, losc will dump it if not in release mode and pass on
*/

static bool __attribute__((noreturn)) config_error(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  syslog(LOG_ERR, "Error while reading configuration!");
  vsyslog(LOG_ERR, fmt, ap);
  va_end(ap);
  closelog();
  exit(1);
}

static char *config_path()
{
  char *cpath = NULL;

  if ((cpath = getenv("LOSC_PATH")))
    asprintf(&cpath, "%s/%s", cpath, LOSC_CONFIG_FILENAME);
  else if ((cpath = getenv("XDG_CONFIG_HOME")))
    asprintf(&cpath, "%s/%s/%s", cpath, LOSC_DIRNAME, LOSC_CONFIG_FILENAME);
  else if ((cpath = getenv("HOME")))
    asprintf(&cpath, "%s/%s/%s/%s", cpath, LOSC_DOTCONFIG, LOSC_DIRNAME, LOSC_CONFIG_FILENAME);
  return cpath;
}

static inline bool config_getlua_boolean(lua_State *L, char *n)
{
  lua_getglobal(L, n);
  if (!lua_isboolean(L, -1))
    config_error("%s is a required boolean", n);
  return lua_toboolean(L, -1);
}

static inline int config_getlua_number(lua_State *L, char *n)
{
  lua_getglobal(L, n);
  if (!lua_isnumber(L, -1))
    config_error("%s is a required integer", n);
  return lua_tonumber(L, -1);
}

static inline char *config_getlua_string(lua_State *L, char *n)
{
  lua_getglobal(L, n);
  if (!lua_isstring(L, -1))
    config_error("%s is a required string", n);
  return strdup((char *)lua_tostring(L, -1));
}

static inline char **config_getlua_array_strings(lua_State *L, char *n)
{
  int len;
  char **ret;

  lua_getglobal(L, n);
  if (!lua_istable(L, -1))
    config_error("%s is a required array", n);
  len = lua_rawlen(L, -1);
  ret = malloc(sizeof(char *) * (len + 1));
  for (int i = 1; i <= len; i++)
    {
      lua_pushnumber(L, i);
      lua_gettable(L, -2);
      if (!lua_isstring(L, -1))
        config_error("%s array must be composed of strings, element %i is not", n, i);
      ret[i - 1] = strdup((char *)lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  ret[len] = NULL;
  return ret;
}

static bool read_config(char *cpath, struct losc_config *config)
{
  lua_State *L;

  L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, cpath) || lua_pcall(L, 0, 0, 0))
    config_error(lua_tostring(L, -1));

  if (!config->atrs)
    config->atrs = config_getlua_array_strings(L, "atrs");
  if (!config->locker)
    config->locker = config_getlua_string(L, "locker");

  lua_close(L);
  return true;
}

bool losc_load_config(char *cp, struct losc_config *config)
{
  if (!cp)
    cp = config_path();
  if (!read_config(cp, config))
    return false;
  free(cp);
  return true;
}
