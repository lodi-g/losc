#pragma once
#ifndef LOSC_H_
# define LOSC_H_

# include <stdbool.h>
# include <syslog.h>

# include "expand_macro.h"
# include "losc_config.h"

# define LOSC_NAME "losc"
# define LOSC_VERSION "0.1"
# define LOSC_AUTHORS "Gregoire Lodi"
# define LOSC_DIRNAME LOSC_NAME

# define UNUSED(x) do { (void)x; } while (0)
# define LIKELY(x) __builtin_expect(x, 1)
# define UNLIKELY(x) __builtin_expect(x, 0)

# define LOSC_LOG_EXIT(s, fmt) \
  do { syslog(LOG_ERR, fmt); closelog(); exit(s); } while (0)

#define _FREE(x) (void)(x && (free(x), 1))

# define xfree(...) \
  do { COMMA_MACRO_EXPAND(_FREE, __VA_ARGS__); } while (0)

void losc_release();
bool losc_daemon_loop(struct losc_config *config);

#endif