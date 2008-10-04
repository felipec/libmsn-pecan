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

#ifndef PECAN_TRANSACTION_H
#define PECAN_TRANSACTION_H

#include <glib.h>

typedef struct PecanTransaction PecanTransaction;

struct PecanTransaction
{
    gint id;
    gint timer;
    gpointer user_data;

    /* command info */
    gchar *command;
    gchar *params;
    gchar *payload;
    gsize payload_len;
};

PecanTransaction *pecan_transaction_new (void);
void pecan_transaction_free (PecanTransaction *trans);
gchar *pecan_transaction_to_string (PecanTransaction *trans);

#endif /* PECAN_TRANSACTION_H */
