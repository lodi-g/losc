#pragma once
#ifndef LOSC_H_
# define LOSC_H_

# include <stdbool.h>

# define LOSC_NAME "losc"
# define LOSC_DIRNAME LOSC_NAME

# define UNUSED(x) (void)x
# define LIKELY(x) __builtin_expect(x, 1)
# define UNLIKELY(x) __builtin_expect(x, 0)

#endif