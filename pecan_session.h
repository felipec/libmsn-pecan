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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#ifndef PECAN_SESSION_H
#define PECAN_SESSION_H

#include <glib-object.h>

typedef struct PecanSession PecanSession;
typedef struct PecanSessionClass PecanSessionClass;
typedef struct PecanSessionPrivate PecanSessionPrivate;

struct PecanSession
{
    GObject parent;

    PecanSessionPrivate *priv;
};

struct PecanSessionClass
{
    GObjectClass parent_class;
};

#define PECAN_SESSION_TYPE (pecan_session_get_type ())
#define PECAN_SESSION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PECAN_SESSION_TYPE, PecanSession))
#define PECAN_SESSION_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), PECAN_SESSION_TYPE, PecanSessionClass))
#define PECAN_SESSION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PECAN_SESSION_TYPE, PecanSessionClass))

PecanSession *pecan_session_new (const gchar *username, const gchar *password);
void pecan_session_free (PecanSession *session);

GType pecan_session_get_type (void);

#endif /* PECAN_SESSION_H */
