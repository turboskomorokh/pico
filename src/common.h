#ifndef _PICO_COMMON_H_
#define _PICO_COMMON_H_

#if defined (__linux__) && !defined (CONFIG_SELF_LIMITS)
#include <linux/limits.h>
#else
#define NR_OPEN	        1024    /* max open file descriptors */

#define NGROUPS_MAX    65536	/* supplemental group IDs are available */
#define ARG_MAX       131072	/* # bytes of args + environ for exec() */
#define LINK_MAX         127	/* # links a file may have */
#define MAX_CANON        255	/* size of the canonical input queue */
#define MAX_INPUT        255	/* size of the type-ahead buffer */
#define NAME_MAX         255	/* # chars in a file name */
#define PATH_MAX         255	/* # chars in a path name including nul */
#define PIPE_BUF         255	/* # bytes in atomic write to a pipe */
#define XATTR_NAME_MAX   255	/* # chars in an extended attribute name */
#define XATTR_SIZE_MAX 65536	/* size of an extended attribute value (64k) */
#define XATTR_LIST_MAX 65536	/* size of extended attribute namelist (64k) */

#define RTSIG_MAX	  32        /* max POSIX realtime signals queued */
#endif

#define TRUE 1
#define FALSE 0

#endif