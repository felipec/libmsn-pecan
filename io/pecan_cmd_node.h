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

#ifndef PECAN_CMD_NODE_H
#define PECAN_CMD_NODE_H

#include <glib-object.h>

typedef struct PecanCmdNode PecanCmdNode;
typedef struct PecanCmdNodeClass PecanCmdNodeClass;
typedef struct PecanCmdNodePrivate PecanCmdNodePrivate;

#include "pecan_node.h"
#include "cmd/pecan_transaction.h"

struct PecanCmdNode
{
    PecanNode parent;

    PecanCmdNodePrivate *priv;
};

struct PecanCmdNodeClass
{
    PecanNodeClass parent_class;
};

#define PECAN_CMD_NODE_TYPE (pecan_cmd_node_get_type ())
#define PECAN_CMD_NODE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PECAN_CMD_NODE_TYPE, PecanCmdNode))

PecanCmdNode *pecan_cmd_node_new (const gchar *name);
void pecan_cmd_node_free (PecanCmdNode *conn);

void pecan_cmd_node_send (PecanCmdNode *cmd_node,
                          PecanTransactionCb cb,
                          const char *command,
                          const char *format, ...);

void pecan_cmd_node_add_cb (PecanCmdNode *cmd_node,
                            const char *command,
                            PecanTransactionCb cb);

GType pecan_cmd_node_get_type (void);

#endif /* PECAN_CMD_NODE_H */
