/**
 * Copyright (C) 2007-2008 Felipe Contreras.
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

#ifndef PECAN_NS_H
#define PECAN_NS_H

#include <glib-object.h>

typedef struct PecanNs PecanNs;
typedef struct PecanNsClass PecanNsClass;
typedef struct PecanNsPrivate PecanNsPrivate;

struct PecanNs
{
    GObject parent;

    PecanNsPrivate *priv;
};

struct PecanNsClass
{
    GObjectClass parent_class;
};

#define PECAN_NS_TYPE (pecan_ns_get_type ())
#define PECAN_NS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PECAN_NS_TYPE, PecanNs))
#define PECAN_NS_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), PECAN_NS_TYPE, PecanNsClass))
#define PECAN_NS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PECAN_NS_TYPE, PecanNsClass))

#include "pecan_session.h"

PecanNs *pecan_ns_new (PecanSession *session);
void pecan_ns_free (PecanNs *ns);
gboolean pecan_ns_connect (PecanNs *ns, const gchar *host, gint port);
void pecan_ns_disconnect (PecanNs *ns);

GType pecan_ns_get_type (void);

#endif /* PECAN_NS_H */
