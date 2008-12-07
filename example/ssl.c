/**
 * Copyright (C) 2008 Felipe Contreras
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

#include "pecan_core.h"
#include "pecan_ssl.h"
#include "io/pecan_socket.h"

#include <netdb.h>
#include <string.h> /* for memcpy */

static PecanSsl *ssl;

void connect_cb (PecanSocket *sock,
                 gboolean success,
                 gpointer user_data)
{
    ssl = pecan_ssl_new ();
    pecan_ssl_connect (ssl, sock->fd);
}

int
main (int argc,
      char *argv[])
{
    pecan_core_init ();

    pecan_socket_connect ("nexus.passport.com", 443, connect_cb, NULL);

    pecan_ssl_free (ssl);

    pecan_core_deinit ();

    return 0;
}
