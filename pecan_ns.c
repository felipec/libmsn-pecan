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

#include "pecan_ns.h"

static gpointer parent_class;

#include "io/pecan_cmd_node.h"

struct PecanNsPrivate
{
    PecanSession *session;
    gchar *host;
    gint port;
};

enum
{
    PROP_SESSION = 1,
};

PecanNs *
pecan_ns_new (PecanSession *session)
{
    PecanNs *ns;

    ns = PECAN_NS (g_object_new (PECAN_NS_TYPE,
                                 "session", session,
                                 NULL));

    return ns;
}

void
pean_ns_free (PecanNs *ns)
{
    if (!ns)
        return;

    g_object_unref (G_OBJECT (ns));
}

static void
instance_init (GTypeInstance *instance,
               gpointer g_class)
{
    PecanNs *self;
    self = PECAN_NS (instance);
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (instance, PECAN_NS_TYPE, PecanNsPrivate);

    g_debug ("instance init");
}

static void
get_property (GObject *object,
              guint property_id,
              GValue *value,
              GParamSpec *spec)
{
    PecanNs *self = PECAN_NS (object);

    switch (property_id)
    {
        case PROP_SESSION:
            g_value_set_object (value, self->priv->session);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, spec);
    }
}

static void
set_property (GObject *object,
              guint property_id,
              const GValue *value,
              GParamSpec *spec)
{
    PecanNs *self = PECAN_NS (object);

    switch (property_id)
    {
        case PROP_SESSION:
            self->priv->session = g_value_get_object (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, spec);
    }
}

static void
dispose (GObject *obj)
{
    G_OBJECT_CLASS (parent_class)->dispose (obj);
}

static void
finalize (GObject *obj)
{
    G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
class_init (gpointer g_class,
            gpointer class_data)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);

    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->get_property = get_property;
    gobject_class->set_property = set_property;

    {
        GParamSpec *param_spec;

        param_spec = g_param_spec_object ("session", "Session", "The Session",
                                          PECAN_SESSION_TYPE,
                                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
        g_object_class_install_property (gobject_class, PROP_SESSION, param_spec);
    }

    parent_class = g_type_class_peek_parent (g_class);
    g_type_class_add_private (g_class, sizeof (PecanNsPrivate));
}

GType
pecan_ns_get_type (void)
{
    static volatile gsize type_volatile = 0;

    if (g_once_init_enter (&type_volatile))
    {
        GType type;
        GTypeInfo *type_info;

        type_info = g_new0 (GTypeInfo, 1);
        type_info->class_size = sizeof (PecanNsClass);
        type_info->class_init = class_init;
        type_info->instance_size = sizeof (PecanNs);
        type_info->instance_init = instance_init;

        type = g_type_register_static (PECAN_CMD_NODE_TYPE, "PecanNsType", type_info, 0);

        g_free (type_info);

        g_once_init_leave (&type_volatile, type);
    }

    return type_volatile;
}
