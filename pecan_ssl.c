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

#include "pecan_ssl.h"
#include "pecan_log.h"

#include <nspr.h>
#include <nss.h>
#include <ssl.h>
#include <private/pprio.h>

struct PecanSsl
{
    PRFileDesc *ssl_fd;
};

gboolean
pecan_ssl_init (void)
{
    SECStatus status;
    PR_Init (PR_SYSTEM_THREAD, PR_PRIORITY_NORMAL, 1);
#if 0
    status = NSS_Init ("/etc/pki/nssdb");
#else
    status = NSS_NoDB_Init (NULL);
#endif
    if (status != SECSuccess)
    {
        pecan_error ("nss init failed");
        return FALSE;
    }

    status = NSS_SetDomesticPolicy ();
    if (status != SECSuccess)
    {
        pecan_error ("set domestic policy failed");
        return FALSE;
    }

    return TRUE;
}

void
pecan_ssl_deinit (void)
{
    NSS_Shutdown ();
    PR_Cleanup ();
}

PecanSsl *
pecan_ssl_new (void)
{
    PecanSsl *ssl;
    ssl = g_new0 (PecanSsl, 1);

    return ssl;
}

static inline const char *
get_error (void)
{
    int code;
    code = PR_GetError ();
    switch (code)
    {
        case PR_INVALID_ARGUMENT_ERROR:
            return "invalid argument";
        case PR_PENDING_INTERRUPT_ERROR:
            return "pending interrupt";
        case PR_IO_PENDING_ERROR:
            return "io pending error";
        case PR_WOULD_BLOCK_ERROR:
            return "would block";
        case PR_IN_PROGRESS_ERROR:
            return "in progress";
        case PR_ALREADY_INITIATED_ERROR:
            return "already initiated";
        case PR_NETWORK_UNREACHABLE_ERROR:
            return "network unreachable";
        case PR_CONNECT_REFUSED_ERROR:
            return "connect refused";
        case PR_CONNECT_TIMEOUT_ERROR:
            return "connect timeout";
        case PR_IO_TIMEOUT_ERROR:
            return "io timeout";
        case PR_NOT_CONNECTED_ERROR:
            return "not connected";
        case PR_CONNECT_RESET_ERROR:
            return "connect reset";
        case PR_IO_ERROR:
            return "io error";
        default:
            pecan_error ("code=%i", code);
            return NULL;
    }
}

static SECStatus
bad_cert_cb (void *arg,
             PRFileDesc *fd)
{
    pecan_error ("bad cert");
    /** @todo handle this. */
    return SECSuccess;
}

void
pecan_ssl_connect (PecanSsl *ssl, gint fd)
{
    SECStatus status;

    ssl->ssl_fd = SSL_ImportFD (NULL, PR_ImportTCPSocket (fd));

    SSL_OptionSet (ssl->ssl_fd, SSL_SECURITY, PR_TRUE);
    SSL_OptionSet (ssl->ssl_fd, SSL_HANDSHAKE_AS_CLIENT, PR_TRUE);

#if 0
    SSL_SetURL (ssl->ssl_fd, "nexus.passport.com");
#endif

    SSL_BadCertHook (ssl->ssl_fd, bad_cert_cb, NULL);

    status = SSL_ResetHandshake (ssl->ssl_fd, PR_FALSE);
    if (status != SECSuccess)
    {
        pecan_error ("ssl reset handshake failed: %s", get_error ());
    }

#if 0
    status = SSL_ForceHandshake (ssl->ssl_fd);
    if (status != SECSuccess)
    {
        pecan_error ("ssl handshake failed: %s", get_error ());
    }
#endif
}

void
pecan_ssl_free (PecanSsl *ssl)
{
    if (!ssl)
        return;

    if (ssl->ssl_fd)
        PR_Close (ssl->ssl_fd);
}

GIOStatus
pecan_ssl_read (PecanSsl *ssl,
                gchar *buf,
                gsize count,
                gsize *bytes_read,
                GError **error)
{
    ssize_t ret;

    ret = PR_Read (ssl->ssl_fd, buf, count);

    if (ret == -1)
        return G_IO_STATUS_ERROR;

    if (bytes_read)
        *bytes_read = ret;

    return (ret == 0) ? G_IO_STATUS_EOF : G_IO_STATUS_NORMAL;
}

GIOStatus
pecan_ssl_write (PecanSsl *ssl,
                 const gchar *buf,
                 gsize count,
                 gsize *bytes_written,
                 GError **error)
{
    ssize_t ret;

    ret = PR_Write (ssl->ssl_fd, buf, count);

    if (ret == -1)
        return G_IO_STATUS_ERROR;

    if (bytes_written)
        *bytes_written = ret;

    return (ret == 0) ? G_IO_STATUS_EOF : G_IO_STATUS_NORMAL;
}
