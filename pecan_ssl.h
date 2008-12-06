/*
 * Copyright (C) 2006-2008 Felipe Contreras
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

#ifndef PECAN_SSL_H
#define PECAN_SSL_H

#include <glib.h>

typedef struct PecanSsl PecanSsl;

gboolean pecan_ssl_init (void);
void pecan_ssl_deinit (void);
PecanSsl *pecan_ssl_new (void);
void pecan_ssl_connect (PecanSsl *ssl, gint fd);
void pecan_ssl_free (PecanSsl *ssl);
GIOStatus pecan_ssl_read (PecanSsl *ssl, gchar *buf, gsize count, gsize *bytes_read, GError **error);
GIOStatus pecan_ssl_write (PecanSsl *ssl, const gchar *buf, gsize count, gsize *bytes_written, GError **error);

#endif /* PECAN_SSL_H */
