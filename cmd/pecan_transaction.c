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

#include "pecan_transaction.h"

PecanTransaction *
pecan_transaction_new (void)
{
    PecanTransaction *trans;

    trans = g_new0 (PecanTransaction, 1);

    return trans;
}

void
pecan_transaction_free (PecanTransaction *trans)
{
    g_free (trans->command);
    g_free (trans->params);
    g_free (trans->payload);

    if (trans->timer)
        g_source_remove (trans->timer);

    g_free (trans);
}

gchar *
pecan_transaction_to_string (PecanTransaction *trans)
{
    gchar *str;

    if (trans->params)
        str = g_strdup_printf ("%s %u %s\r\n", trans->command, trans->id, trans->params);
    else
        str = g_strdup_printf ("%s %u\r\n", trans->command, trans->id);

    return str;
}
