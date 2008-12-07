/*
 * Copyright (C) 2004-2008 Felipe Contreras.
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "pecan_command.h"

#include <string.h>
#include <stdlib.h> /* for atoi */

static inline gboolean
is_num (const char *str)
{
    const char *c;
    for (c = str; *c; c++)
    {
        if (!(g_ascii_isdigit (*c)))
            return FALSE;
    }

    return TRUE;
}

PecanCommand *
pecan_command_new_from_string (const char *string)
{
    PecanCommand *cmd;
    char *param_start;

    cmd = g_new0 (PecanCommand, 1);

    cmd->base = g_strdup (string);
    param_start = strchr (cmd->base, ' ');

    /** @todo check string "preferredEmail: " */

    if (param_start)
    {
        *param_start++ = '\0';
        cmd->paramv = g_strsplit (param_start, " ", 0);
    }

    if (cmd->paramv && cmd->paramv[0])
    {
        const char *param;
        int c;

        for (c = 0; cmd->paramv[c]; c++);
        cmd->paramc = c;

        param = cmd->paramv[0];

        cmd->tr_id = is_num (param) ? atoi (param) : 0;
    }
    else
        cmd->tr_id = 0;

    return cmd;
}

void
pecan_command_free (PecanCommand *cmd)
{
    g_free (cmd->payload);
    g_free (cmd->base);
    g_strfreev (cmd->paramv);

    g_free (cmd);
}
