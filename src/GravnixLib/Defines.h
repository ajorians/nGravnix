#ifndef DEFINES_H
#define DEFINES_H

#ifndef DEBUG_MSG
#define DEBUG_MSG//     printf
#endif

#ifndef DEBUG_FUNC_NAME
#define DEBUG_FUNC_NAME	DEBUG_MSG("%s: %s (%d)\n", __FILE__, __func__, __LINE__);
#endif

#endif

