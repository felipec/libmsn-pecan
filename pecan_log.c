/**
 * Copyright (C) 2007-2008 Felipe Contreras
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#include "pecan_log.h"
#include "pecan_printf.h"

#ifdef PECAN_DEBUG

#include <fcntl.h>
#include <unistd.h>

static inline const gchar *
log_level_to_string (PecanLogLevel level)
{
    switch (level)
    {
        case PECAN_LOG_LEVEL_NONE: return "NONE"; break;
        case PECAN_LOG_LEVEL_ERROR: return "ERROR"; break;
        case PECAN_LOG_LEVEL_WARNING: return "WARNING"; break;
        case PECAN_LOG_LEVEL_INFO: return "INFO"; break;
        case PECAN_LOG_LEVEL_DEBUG: return "DEBUG"; break;
        case PECAN_LOG_LEVEL_LOG: return "LOG"; break;
        default: return "Unknown"; break;
    }
}

void
pecan_base_log_helper (guint level,
		       const gchar *file,
		       const gchar *function,
		       gint line,
		       const gchar *fmt,
		       ...)
{
    gchar *tmp;
    va_list args;

    if (level > PECAN_LOG_LEVEL_INFO)
        return;

    va_start (args, fmt);

    tmp = pecan_strdup_vprintf (fmt, args);

    pecan_print ("%s %s:%d:%s() %s\n",
                 log_level_to_string (level),
                 file, line, function,
                 tmp);
    g_free (tmp);

    va_end (args);
}

#endif /* PECAN_DEBUG */
