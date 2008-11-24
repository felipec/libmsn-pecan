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

#include "pecan_session.h"
#include "pecan_log.h"

#include <msn/msn.h>

#include "cb.h"

static gpointer parent_class;

struct PecanSessionPrivate
{
    gchar *username;
    gchar *password;
    MSN::NotificationServerConnection *ns;
    Callbacks *cb;
};

enum
{
    PROP_USERNAME = 1,
    PROP_PASSWORD,
};

G_BEGIN_DECLS

PecanSession *
pecan_session_new (const gchar *username,
                   const gchar *password)
{
    PecanSession *session;

    session = PECAN_SESSION (g_object_new (PECAN_SESSION_TYPE,
                                           "username", username,
                                           "password", password,
                                           NULL));

    return session;
}

void
pecan_session_free (PecanSession *session)
{
    if (!session)
        return;
    g_object_unref (G_OBJECT (session));
}

void
pecan_session_connect (PecanSession *session,
                       const gchar *host,
                       gint port)
{
    MSN::NotificationServerConnection *ns;
#if 0
    MSN::Passport passport (session->priv->username);
    session->priv->ns = ns = new MSN::NotificationServerConnection (passport, session->priv->password, session->priv->cb);
#else
    printf ("%s, %i\n", host, port);
    printf ("%s, %s\n", session->priv->username, session->priv->password);
    MSN::Passport passport ("flikazo@hotmail.com");
    session->priv->cb = new Callbacks ();
    session->priv->ns = ns = new MSN::NotificationServerConnection (passport, "felipe", *session->priv->cb);
    session->priv->cb->ns = ns;
#endif
    if (ns)
        ns->connect (host, port);
}

void
pecan_session_disconnect (PecanSession *session)
{
    MSN::NotificationServerConnection *ns;

    ns = session->priv->ns;

    if (ns)
    {
        if (ns->connectionState () != MSN::NotificationServerConnection::NS_DISCONNECTED)
        {
            delete ns;
            ns = NULL;
        }
    }
}

const gchar *
pecan_session_get_username (PecanSession *session)
{
    return session->priv->username;
}

G_END_DECLS

/* GObject stuff. */

static void
instance_init (GTypeInstance *instance,
               gpointer g_class)
{
    PecanSession *self;
    self = PECAN_SESSION (instance);
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (instance, PECAN_SESSION_TYPE, PecanSessionPrivate);
}

static void
get_property (GObject *object,
              guint property_id,
              GValue *value,
              GParamSpec *spec)
{
    PecanSession *self = PECAN_SESSION (object);

    switch (property_id)
    {
        case PROP_USERNAME:
            g_value_set_string (value, self->priv->username);
            break;
        case PROP_PASSWORD:
            g_value_set_string (value, self->priv->password);
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
    PecanSession *self = PECAN_SESSION (object);

    switch (property_id)
    {
        case PROP_USERNAME:
            g_free (self->priv->username);
            self->priv->username = g_utf8_strdown (g_value_get_string (value), -1);
            break;
        case PROP_PASSWORD:
            g_free (self->priv->password);
            self->priv->password = g_strdup (g_value_get_string (value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, spec);
    }
}

static void
dispose (GObject *obj)
{
    PecanSession *self;
    self = PECAN_SESSION (obj);

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

#if 0
    {
        GParamSpec *param_spec;

        param_spec = g_param_spec_string ("username", "Username",
                                          "The username", NULL,
                                          (GParamFlags) G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
        g_object_class_install_property (gobject_class, PROP_USERNAME, param_spec);

        param_spec = g_param_spec_string ("password", "Password",
                                          "The password", NULL,
                                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
        g_object_class_install_property (gobject_class, PROP_PASSWORD, param_spec);
    }
#endif

    {
        PecanSessionClass *session_class = PECAN_SESSION_CLASS (g_class);
        session_class->error_sig = g_signal_new ("error", G_TYPE_FROM_CLASS (gobject_class),
                                                 G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                                                 g_cclosure_marshal_VOID__POINTER,
                                                 G_TYPE_NONE, 1, G_TYPE_POINTER);
    }

    parent_class = g_type_class_peek_parent (g_class);
    g_type_class_add_private (g_class, sizeof (PecanSessionPrivate));
}

GType
pecan_session_get_type (void)
{
    static volatile gsize type_volatile = 0;

    if (g_once_init_enter (&type_volatile))
    {
        GType type;
        GTypeInfo *type_info;

        type_info = g_new0 (GTypeInfo, 1);
        type_info->class_size = sizeof (PecanSessionClass);
        type_info->class_init = class_init;
        type_info->instance_size = sizeof (PecanSession);
        type_info->instance_init = instance_init;

        type = g_type_register_static (G_TYPE_OBJECT, "PecanSessionType", type_info, (GTypeFlags) 0);

        g_free (type_info);

        g_once_init_leave (&type_volatile, type);
    }

    return type_volatile;
}