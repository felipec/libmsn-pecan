/*
 * Copyright (C) 2006-2008 Felipe Contreras.
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

#include "pecan_node.h"
#include "pecan_stream.h"
#include "pecan_log.h"
#ifdef PECAN_SOCKET
#include "io/pecan_socket.h"
#endif /* PECAN_SOCKET */
#include "pecan_config.h"

void pecan_node_error (PecanNode *conn);

static gpointer parent_class;

enum
{
    PROP_NAME = 1,
};

struct PecanNodePrivate
{
    GError *error; /**< The current IO error .*/
    guint read_watch; /** < The source id of the read watch. */

    gchar *name;

    gpointer data; /**< Client data. */
    gpointer foo_data;
    PecanNode *prev;
    PecanNode *next;

    PecanStream *stream; /**< The current IO stream .*/

    gchar *hostname;
    guint port;

    struct _PurpleProxyConnectData *connect_data;
    struct MsnSession *session;
    gulong open_sig_handler;
    gulong close_sig_handler;
    gulong error_sig_handler;

    gboolean dump_file;
};

GQuark
pecan_node_error_quark (void)
{
    return g_quark_from_static_string ("pecan-node-error-quark");
}

static inline const gchar *
status_to_str (GIOStatus status)
{
    const gchar *id;

    switch (status)
    {
        case G_IO_STATUS_ERROR: id = "ERROR"; break;
        case G_IO_STATUS_NORMAL: id = "NORMAL"; break;
        case G_IO_STATUS_EOF: id = "EOF"; break;
        case G_IO_STATUS_AGAIN: id = "AGAIN"; break;
        default: id = NULL; break;
    }

    return id;
}

static gboolean
read_cb (GIOChannel *source,
         GIOCondition condition,
         gpointer data)
{
    PecanNode *conn;
    PecanNodePrivate *priv;
    gchar buf[PECAN_BUF_LEN + 1];
    gsize bytes_read;

    pecan_log ("begin");

    conn = PECAN_NODE (data);
    priv = conn->priv;

    pecan_debug ("conn=%p,name=%s", conn, priv->name);

    g_object_ref (conn);

    {
        GIOStatus status = G_IO_STATUS_NORMAL;

        status = pecan_node_read (conn, buf, PECAN_BUF_LEN, &bytes_read, NULL);

        if (status == G_IO_STATUS_AGAIN)
        {
            g_object_unref (conn);
            return TRUE;
        }

        if (!priv->error && status == G_IO_STATUS_EOF)
        {
            pecan_warning ("should not happen");
            priv->error = g_error_new (PECAN_NODE_ERROR, PECAN_NODE_ERROR_READ, "End of stream");
        }

        if (priv->error)
        {
            pecan_node_error (conn);
            g_object_unref (conn);
            return FALSE;
        }
    }

    pecan_node_parse (conn, buf, bytes_read);

    g_object_unref (conn);

    pecan_log ("end");

    return TRUE;
}

static void
open_cb (PecanNode *next,
         gpointer data)
{
    PecanNode *conn;
    PecanNodePrivate *priv;

    conn = PECAN_NODE (data);
    priv = conn->priv;

    pecan_log ("begin");

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->open_sig, 0, conn);
    }

    g_signal_handler_disconnect (next, priv->open_sig_handler);
    priv->open_sig_handler = 0;

    pecan_log ("end");
}

static void
close_cb (PecanNode *next,
          gpointer data)
{
    PecanNode *conn;

    conn = PECAN_NODE (data);

    pecan_log ("begin");

    pecan_node_close (conn);

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->close_sig, 0, conn);
    }

    pecan_log ("end");
}

static void
error_cb (PecanNode *next,
          gpointer arg,
          gpointer data)
{
    PecanNode *conn;
    PecanNodePrivate *priv;

    conn = PECAN_NODE (data);
    priv = conn->priv;

    pecan_log ("begin");

    if (next->priv->error)
    {
        g_propagate_error (&priv->error, next->priv->error);
        next->priv->error = NULL;
    }

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->error_sig, 0, arg);
    }

    pecan_log ("end");
}

PecanNode *
pecan_node_new (gchar *name)
{
    PecanNode *conn;
    PecanNodePrivate *priv;

    pecan_log ("begin");

    conn = PECAN_NODE (g_type_create_instance (PECAN_NODE_TYPE));
    priv = conn->priv;

    priv->name = g_strdup (name);

    pecan_log ("end");

    return conn;
}

void
pecan_node_free (PecanNode *conn)
{
    g_return_if_fail (conn != NULL);
    pecan_log ("begin");
    g_object_unref (G_OBJECT (conn));
    pecan_log ("end");
}

void
pecan_node_error (PecanNode *conn)
{
    PecanNodePrivate *priv;

    g_return_if_fail (conn != NULL);

    pecan_debug ("conn=%p", conn);
    priv = conn->priv;

    g_object_ref (conn);

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->error_sig, 0, priv->error);
    }

    if (priv->error)
    {
        pecan_warning ("unhandled error: %s", priv->error->message);
        g_clear_error (&priv->error);
    }

    g_object_unref (conn);
}

GIOStatus
pecan_node_write (PecanNode *conn,
                  const gchar *buf,
                  gsize count,
                  gsize *ret_bytes_written,
                  GError **error)
{
    return PECAN_NODE_GET_CLASS (conn)->write (conn, buf, count, ret_bytes_written, error);
}

GIOStatus
pecan_node_read (PecanNode *conn,
                 gchar *buf,
                 gsize count,
                 gsize *ret_bytes_read,
                 GError **error)
{
    return PECAN_NODE_GET_CLASS (conn)->read (conn, buf, count, ret_bytes_read, error);
}

/* If two nodes are linked the 'next' node is used for the real communication. */
void
pecan_node_link (PecanNode *conn,
                 PecanNode *next)
{
    PecanNodePrivate *priv;

    priv = conn->priv;

    priv->next = g_object_ref (next);
    priv->open_sig_handler = g_signal_connect (next, "open", G_CALLBACK (open_cb), conn);
    priv->close_sig_handler = g_signal_connect (next, "close", G_CALLBACK (close_cb), conn);
    priv->error_sig_handler = g_signal_connect (next, "error", G_CALLBACK (error_cb), conn);
}

void
pecan_node_connect (PecanNode *conn,
                    const gchar *hostname,
                    gint port)
{
    PECAN_NODE_GET_CLASS (conn)->connect (conn, hostname, port);
}

void
pecan_node_close (PecanNode *conn)
{
    PECAN_NODE_GET_CLASS (conn)->close (conn);
}

void
pecan_node_parse (PecanNode *conn,
                  gchar *buf,
                  gsize bytes_read)
{
    PECAN_NODE_GET_CLASS (conn)->parse (conn, buf, bytes_read);
}

const gchar *
pecan_node_get_name (PecanNode *conn)
{
    return conn->priv->name;
}

/* PecanNode stuff. */

#ifdef PECAN_SOCKET
static void
connect_cb (PecanSocket *sock,
            gboolean success,
            gpointer user_data)
{
    PecanNode *conn;
    PecanNodePrivate *priv;

    pecan_log ("begin");

    conn = PECAN_NODE (user_data);
    priv = conn->priv;

    priv->connect_data = NULL;

    g_object_ref (conn);

    if (success)
    {
        GIOChannel *channel;

        priv->stream = pecan_stream_new (sock->fd);
        channel = priv->stream->channel;

        g_io_channel_set_encoding (channel, NULL, NULL);
        g_io_channel_set_buffered (channel, FALSE);

        pecan_info ("connected: conn=%p,channel=%p", conn, channel);
        priv->read_watch = g_io_add_watch (channel, G_IO_IN, read_cb, conn);
#if 0
        g_io_add_watch (channel, G_IO_ERR | G_IO_HUP | G_IO_NVAL, close_cb, conn);
#endif
    }
    else
    {
        priv->error = g_error_new_literal (PECAN_NODE_ERROR, PECAN_NODE_ERROR_OPEN,
                                           "Unable to connect");

        pecan_node_error (conn);
    }

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->open_sig, 0, conn);
    }

    g_object_unref (conn);

    pecan_log ("end");
}
#else
static void
connect_cb (gpointer data,
            gint source,
            const gchar *error_message)
{
    PecanNode *conn;

    pecan_log ("begin");

    conn = PECAN_NODE (data);
    priv->connect_data = NULL;

    g_object_ref (conn);

    if (source >= 0)
    {
        GIOChannel *channel;

        priv->stream = pecan_stream_new (source);
        channel = priv->stream->channel;

        g_io_channel_set_encoding (channel, NULL, NULL);
        g_io_channel_set_buffered (channel, FALSE);

        pecan_info ("connected: conn=%p,channel=%p", conn, channel);
        priv->read_watch = g_io_add_watch (channel, G_IO_IN, read_cb, conn);
#if 0
        g_io_add_watch (channel, G_IO_ERR | G_IO_HUP | G_IO_NVAL, close_cb, conn);
#endif
    }
    else
    {
        /* pecan_error ("connection error: conn=%p,msg=[%s]", conn, error_message); */
        priv->error = g_error_new_literal (PECAN_NODE_ERROR, PECAN_NODE_ERROR_OPEN,
                                           error_message ? error_message : "Unable to connect");

        pecan_node_error (conn);
    }

    {
        PecanNodeClass *class;
        class = g_type_class_peek (PECAN_NODE_TYPE);
        g_signal_emit (G_OBJECT (conn), class->open_sig, 0, conn);
    }

    g_object_unref (conn);

    pecan_log ("end");
}
#endif /* PECAN_SOCKET */

static void
connect_impl (PecanNode *conn,
              const gchar *hostname,
              gint port)
{
    PecanNodePrivate *priv;

    g_return_if_fail (conn);

    pecan_log ("begin");
    priv = conn->priv;

    pecan_debug ("conn=%p,name=%s", conn, priv->name);
    pecan_debug ("hostname=%s,port=%d", hostname, port);
    pecan_debug ("next=%p", priv->next);

    g_free (priv->hostname);
    priv->hostname = g_strdup (hostname);
    priv->port = port;

    if (priv->next)
    {
        priv->next->priv->prev = conn;
        pecan_node_connect (priv->next, hostname, port);
        priv->next->priv->prev = NULL;
    }
    else
    {
        pecan_node_close (conn);

#ifdef PECAN_SOCKET
        pecan_socket_connect (hostname, port, connect_cb, conn);
#endif
    }

    pecan_log ("end");
}

static void
close_impl (PecanNode *conn)
{
    PecanNodePrivate *priv;

    g_return_if_fail (conn);

    pecan_log ("begin");
    priv = conn->priv;

    pecan_log ("conn=%p,name=%s", conn, priv->name);

    if (priv->next)
    {
        pecan_node_close (priv->next);
    }

    g_free (priv->hostname);
    priv->hostname = NULL;

    if (!priv->stream)
    {
        pecan_warning ("not connected: conn=%p", conn);
    }

    if (priv->read_watch)
    {
        g_source_remove (priv->read_watch);
        priv->read_watch = 0;
    }

    if (priv->stream)
    {
        pecan_info ("stream shutdown: %p", priv->stream);
        pecan_stream_free (priv->stream);
        priv->stream = NULL;
    }

    pecan_log ("end");
}

static void
error_impl (PecanNode *conn)
{
    pecan_info ("foo");
}

static GIOStatus
write_impl (PecanNode *conn,
            const gchar *buf,
            gsize count,
            gsize *ret_bytes_written,
            GError **error)
{
    PecanNodePrivate *priv;
    GIOStatus status = G_IO_STATUS_NORMAL;

    priv = conn->priv;
    pecan_debug ("name=%s", priv->name);

    if (priv->next)
    {
        PecanNode *next;

        next = priv->next;

        /* priv->next has already a ref from conn, but let's just be sure and
         * ref anyway */
        g_object_ref (next);
        next->priv->prev = conn;
        status = pecan_node_write (next, buf, count, ret_bytes_written, error);
        next->priv->prev = NULL;
        g_object_unref (next);
    }
    else
    {
        GError *tmp_error = NULL;
        gsize bytes_written = 0;

        pecan_debug ("stream=%p", priv->stream);

        status = pecan_stream_write_full (priv->stream, buf, count, &bytes_written, &tmp_error);

        pecan_log ("bytes_written=%d", bytes_written);

        if (status == G_IO_STATUS_NORMAL)
        {
            if (bytes_written < count)
            {
                /* This shouldn't happen, right? */
                /* It doesn't seem to happen, but keep checking for now. */
                pecan_error ("write check: %d < %d", bytes_written, count);
            }
        }
        else
        {
            pecan_warning ("not normal: status=%d (%s)",
                           status, status_to_str (status));
        }

        if (ret_bytes_written)
            *ret_bytes_written = bytes_written;

        if (tmp_error)
        {
            priv->error = g_error_copy (tmp_error);
            g_propagate_error (error, tmp_error);
        }
    }

    return status;
}

static GIOStatus
read_impl (PecanNode *conn,
           gchar *buf,
           gsize count,
           gsize *ret_bytes_read,
           GError **error)
{
    PecanNodePrivate *priv;
    GIOStatus status = G_IO_STATUS_NORMAL;

    priv = conn->priv;
    pecan_debug ("name=%s", priv->name);

    if (priv->next)
    {
        pecan_error ("whaaat");
        priv->next->priv->prev = conn;
        status = pecan_node_read (priv->next, buf, count, ret_bytes_read, error);
        priv->next->priv->prev = NULL;
    }
    else
    {
        GError *tmp_error = NULL;
        gsize bytes_read = 0;

        pecan_debug ("stream=%p", priv->stream);

        status = pecan_stream_read (priv->stream, buf, count, &bytes_read, &tmp_error);

        if (status != G_IO_STATUS_NORMAL)
        {
            pecan_info ("not normal: status=%d (%s)",
                        status, status_to_str (status));
        }

        pecan_log ("bytes_read=%d", bytes_read);

        if (ret_bytes_read)
            *ret_bytes_read = bytes_read;

        if (tmp_error)
        {
            priv->error = g_error_copy (tmp_error);
            g_propagate_error (error, tmp_error);
        }
    }

    return status;
}

static void
parse_impl (PecanNode *conn,
            gchar *buf,
            gsize bytes_read)
{
    PecanNodePrivate *priv;
    priv = conn->priv;
    pecan_debug ("name=%s", priv->name);
}

/* GObject stuff. */

static void
get_property (GObject *object,
              guint property_id,
              GValue *value,
              GParamSpec *spec)
{
    PecanNode *self = PECAN_NODE (object);

    switch (property_id)
    {
        case PROP_NAME:
            g_value_set_string (value, self->priv->name);
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
    PecanNode *self = PECAN_NODE (object);

    switch (property_id)
    {
        case PROP_NAME:
            g_free (self->priv->name);
            self->priv->name = g_strdup (g_value_get_string (value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, spec);
    }
}

static void
dispose (GObject *obj)
{
    PecanNode *conn;
    PecanNodePrivate *priv;

    pecan_log ("begin");
    conn = PECAN_NODE (obj);
    priv = conn->priv;

    if (priv->next)
    {
        g_signal_handler_disconnect (priv->next, priv->open_sig_handler);
        g_signal_handler_disconnect (priv->next, priv->close_sig_handler);
        g_signal_handler_disconnect (priv->next, priv->error_sig_handler);
        pecan_node_free (priv->next);
        priv->next = NULL;
    }

    if (!conn->dispose_has_run)
    {
        conn->dispose_has_run = TRUE;

        pecan_node_close (conn);

        g_free (priv->name);
    }

    G_OBJECT_CLASS (parent_class)->dispose (obj);

    pecan_log ("end");
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
    PecanNodeClass *conn_class = PECAN_NODE_CLASS (g_class);
    GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);

    conn_class->connect = &connect_impl;
    conn_class->close = &close_impl;
    conn_class->error = &error_impl;
    conn_class->write = &write_impl;
    conn_class->read = &read_impl;
    conn_class->parse = &parse_impl;

    gobject_class->get_property = get_property;
    gobject_class->set_property = set_property;
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;

    conn_class->open_sig = g_signal_new ("open", G_TYPE_FROM_CLASS (gobject_class),
                                         G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                                         g_cclosure_marshal_VOID__VOID,
                                         G_TYPE_NONE, 0);

    conn_class->close_sig = g_signal_new ("close", G_TYPE_FROM_CLASS (gobject_class),
                                          G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                                          g_cclosure_marshal_VOID__VOID,
                                          G_TYPE_NONE, 0);

    conn_class->error_sig = g_signal_new ("error", G_TYPE_FROM_CLASS (gobject_class),
                                          G_SIGNAL_RUN_FIRST, 0, NULL, NULL,
                                          g_cclosure_marshal_VOID__POINTER,
                                          G_TYPE_NONE, 1, G_TYPE_POINTER);

    {
        GParamSpec *param_spec;

        param_spec = g_param_spec_string ("name", "Name", "The identification of this node",
                                          NULL,
                                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
        g_object_class_install_property (gobject_class, PROP_NAME, param_spec);
    }

    parent_class = g_type_class_peek_parent (g_class);
    g_type_class_add_private (g_class, sizeof (PecanNodePrivate));
}

static void
instance_init (GTypeInstance *instance,
               gpointer g_class)
{
    PecanNode *self;
    self = PECAN_NODE (instance);
    self->priv = G_TYPE_INSTANCE_GET_PRIVATE (instance, PECAN_NODE_TYPE, PecanNodePrivate);
}

GType
pecan_node_get_type (void)
{
    static volatile gsize type_volatile = 0;

    if (g_once_init_enter (&type_volatile))
    {
        GType type;
        GTypeInfo *type_info;

        type_info = g_new0 (GTypeInfo, 1);
        type_info->class_size = sizeof (PecanNodeClass);
        type_info->class_init = class_init;
        type_info->instance_size = sizeof (PecanNode);
        type_info->instance_init = instance_init;

        type = g_type_register_static (G_TYPE_OBJECT, "PecanNodeType", type_info, 0);

        g_free (type_info);

        g_once_init_leave (&type_volatile, type);
    }

    return type_volatile;
}
