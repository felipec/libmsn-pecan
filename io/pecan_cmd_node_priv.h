/*
 * Copyright (C) 2008 Felipe Contreras.
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

#ifndef PECAN_CMD_NODE_PRIV_H
#define PECAN_CMD_NODE_PRIV_H

#include <glib-object.h>

typedef struct PecanCmdNodeClass PecanCmdNodeClass;

#include "pecan_node_priv.h"
#include "pecan_cmd_node.h"

struct PecanCmdNode
{
    PecanNode parent;

    gsize payload_len;
    gchar *rx_buf;
    gsize rx_len;

#if 0
    struct MsnCmdProc *cmdproc;
#endif
};

struct PecanCmdNodeClass
{
    PecanNodeClass parent_class;
};

GType pecan_cmd_node_get_type (void);

#endif /* PECAN_CMD_NODE_PRIV_H */
