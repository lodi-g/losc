#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "losc.h"
#include "losc_config.h"

extern char *program_invocation_name;

static bool config_error(const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  fprintf(stderr, "%s: Error while reading configuration: ", program_invocation_name);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, ".\n");
  va_end(ap);
  exit(1);
}

static char *config_path()
{
  char *cpath;

  if ((cpath = getenv("LOSC_PATH")))
    return cpath;
  if ((cpath = getenv("XDG_CONFIG_HOME")))
    return cpath;
  if ((cpath = getenv("HOME")))
    {
      asprintf(&cpath, "%s/%s/%s", getenv("HOME"), LOSC_DOTCONFIG, LOSC_DIRNAME);
      return cpath;
    }
  return NULL;
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

static struct losc_config *read_config(char *cpath)
{
  lua_State *L;
  struct losc_config *config = malloc(sizeof(struct losc_config));

  L = luaL_newstate();
  luaL_openlibs(L);
  if (luaL_loadfile(L, cpath) || lua_pcall(L, 0, 0, 0))
    config_error(lua_tostring(L, -1));

  config->b = config_getlua_boolean(L, "b");
  config->var = config_getlua_number(L, "var");
  config->str = config_getlua_string(L, "str");

  lua_close(L);

  printf("%s\n", config->b ? "true" : "false");
  printf("%i\n", config->var);
  printf("%s\n", config->str);
  return config;
}

struct losc_config *losc_load_config(char *cp)
{
  struct losc_config *config;
  char *cpath;
  char *cfp;

  if (!(cpath = cp ? cp : config_path()))
    return NULL;
  asprintf(&cfp, "%s/%s", cpath, "config");
  config = read_config(cfp);
  xfree(cpath, cfp);
  return config;
}
