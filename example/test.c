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

#include "pecan_session.h"

GMainLoop *loop;

static gboolean
idle (gpointer data)
{
    PecanSession *session;
    session = (PecanSession *) data;

    pecan_session_connect (session, "messenger.hotmail.com", 1863);
    g_debug ("connecting");

    return FALSE;
}

int
main (int argc,
      char *argv[])
{
    PecanSession *session;

    g_type_init ();
    /* g_thread_init (NULL); */
    loop = g_main_loop_new (NULL, FALSE);

    session = pecan_session_new (argv[1], argv[2]);

    g_idle_add (idle, session);
    g_main_loop_run (loop);

    pecan_session_free (session);

    return 0;
}
