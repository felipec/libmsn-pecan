#include "cb.h"

#include "io/pecan_socket.h"

static gboolean
read_cb (GIOChannel *source,
         GIOCondition condition,
         gpointer data)
{
    Callbacks *foo;
    printf ("%s\n", __func__);
    foo = (Callbacks *) data;
    foo->ns->dataArrivedOnSocket();

    return TRUE;
}

static void
connect_cb (PecanSocket *sock,
            gboolean success,
            gpointer user_data)
{
    Callbacks *foo;
    printf ("%s: success=%i\n", __func__,
            success);
    foo = (Callbacks *) user_data;
    foo->fd = sock->fd;

    if (success)
    {
        GIOChannel *channel;

        channel = g_io_channel_unix_new (sock->fd);

        g_io_channel_set_encoding (channel, NULL, NULL);
        g_io_channel_set_buffered (channel, FALSE);

        g_io_add_watch (channel, G_IO_IN, read_cb, user_data);
    }
}

void
Callbacks::registerSocket (void *s,
                           int reading,
                           int writing,
                           bool isSSL)
{
    printf ("%s: s=%p, reading=%i, writing=%i, isSSL=%i\n", __func__,
            s, reading, writing, isSSL);
}

void
Callbacks::closeSocket (void *s)
{
    printf ("%s\n", __func__);
}

void
Callbacks::unregisterSocket (void *s)
{
    printf ("%s\n", __func__);
}

void
Callbacks::gotFriendlyName (MSN::NotificationServerConnection *conn,
                            std::string friendlyname)
{
}

void
Callbacks::fileTransferInviteResponse (MSN::SwitchboardServerConnection *conn,
                                       unsigned int sessionID,
                                       bool response)
{
}

void
Callbacks::gotContactDisplayPicture (MSN::SwitchboardServerConnection *conn,
                                     MSN::Passport passport,
                                     std::string filename)
{
}

void
Callbacks::gotMessageSentACK (MSN::SwitchboardServerConnection *conn,
                              int trID)
{
}

void
Callbacks::gotBuddyListInfo (MSN::NotificationServerConnection *conn,
                             MSN::ListSyncInfo *info)
{
}

void
Callbacks::gotLatestListSerial (MSN::NotificationServerConnection *conn,
                                std::string lastChange)
{
}

void
Callbacks::gotGTC (MSN::NotificationServerConnection *conn,
                   char c)
{
}

void
Callbacks::gotOIMDeleteConfirmation (MSN::NotificationServerConnection *conn,
                                     bool success,
                                     std::string id)
{
}

void
Callbacks::gotOIMSendConfirmation (MSN::NotificationServerConnection *conn,
                                   bool success,
                                   int id)
{
}

void
Callbacks::gotOIM (MSN::NotificationServerConnection *conn,
                   bool success,
                   std::string id,
                   std::string message)
{
}

void
Callbacks::gotOIMList (MSN::NotificationServerConnection *conn,
                       std::vector < MSN::eachOIM > OIMs)
{
}

void
Callbacks::connectionReady (MSN::Connection *conn)
{
}

void
Callbacks::gotBLP (MSN::NotificationServerConnection *conn,
                   char c)
{
}

void
Callbacks::addedListEntry (MSN::NotificationServerConnection *conn,
                           MSN::ContactList list,
                           MSN::Passport username,
                           std::string friendlyname)
{
}

void
Callbacks::removedListEntry (MSN::NotificationServerConnection *conn,
                             MSN::ContactList list,
                             MSN::Passport username)
{
}

void
Callbacks::addedGroup (MSN::NotificationServerConnection *conn,
                       bool added,
                       std::string groupName,
                       std::string groupID)
{
}

void
Callbacks::removedGroup (MSN::NotificationServerConnection *conn,
                         bool removed,
                         std::string groupID)
{
}

void
Callbacks::renamedGroup (MSN::NotificationServerConnection *conn,
                         bool renamed,
                         std::string newGroupName,
                         std::string groupID)
{
}

void
Callbacks::showError (MSN::Connection *conn,
                      std::string msg)
{
}

void
Callbacks::buddyChangedStatus (MSN::NotificationServerConnection *conn,
                               MSN::Passport buddy,
                               std::string friendlyname,
                               MSN::BuddyStatus status,
                               unsigned int clientID,
                               std::string msnobject)
{
}

void
Callbacks::buddyOffline (MSN::NotificationServerConnection *conn,
                         MSN::Passport buddy)
{
}

void
Callbacks::gotSwitchboard (MSN::SwitchboardServerConnection *conn,
                           const void *tag)
{
}

void
Callbacks::buddyJoinedConversation (MSN::SwitchboardServerConnection *conn,
                                    MSN::Passport username,
                                    std::string friendlyname,
                                    int is_initial)
{
}

void
Callbacks::buddyLeftConversation (MSN::SwitchboardServerConnection *conn,
                                  MSN::Passport username)
{
}

void
Callbacks::gotInstantMessage (MSN::SwitchboardServerConnection *conn,
                              MSN::Passport username,
                              std::string friendlyname,
                              MSN::Message *msg)
{
}

void
Callbacks::gotEmoticonNotification (MSN::SwitchboardServerConnection *conn,
                                    MSN::Passport username,
                                    std::string alias,
                                    std::string msnobject)
{
}

void
Callbacks::failedSendingMessage (MSN::Connection *conn)
{
}

void
Callbacks::buddyTyping (MSN::SwitchboardServerConnection *conn,
                        MSN::Passport username,
                        std::string friendlyname)
{
}

void
Callbacks::gotNudge (MSN::SwitchboardServerConnection *conn,
                     MSN::Passport username)
{
}

void
Callbacks::gotVoiceClipNotification (MSN::SwitchboardServerConnection *conn,
                                     MSN::Passport username,
                                     std::string msnobject)
{
}

void
Callbacks::gotWinkNotification (MSN::SwitchboardServerConnection *conn,
                                MSN::Passport username,
                                std::string msnobject)
{
}

void
Callbacks::gotInk (MSN::SwitchboardServerConnection *conn,
                   MSN::Passport username,
                   std::string image)
{
}

void
Callbacks::gotActionMessage (MSN::SwitchboardServerConnection *conn,
                             MSN::Passport username,
                             std::string message)
{
}

void
Callbacks::gotInitialEmailNotification (MSN::NotificationServerConnection *conn,
                                        int msgs_inbox,
                                        int unread_inbox,
                                        int msgs_folders,
                                        int unread_folders)
{
}

void
Callbacks::gotNewEmailNotification (MSN::NotificationServerConnection *conn,
                                    std::string from,
                                    std::string subject)
{
}

void
Callbacks::fileTransferProgress (MSN::SwitchboardServerConnection *conn,
                                 unsigned int sessionID,
                                 unsigned long long transferred,
                                 unsigned long long total)
{
}

void
Callbacks::fileTransferFailed (MSN::SwitchboardServerConnection *conn,
                               unsigned int sessionID,
                               MSN::fileTransferError error)
{
}

void
Callbacks::fileTransferSucceeded (MSN::SwitchboardServerConnection *conn,
                                  unsigned int sessionID)
{
}

void
Callbacks::gotNewConnection (MSN::Connection *conn)
{
}

void
Callbacks::buddyChangedPersonalInfo (MSN::NotificationServerConnection *conn,
                                     MSN::Passport fromPassport,
                                     MSN::personalInfo pInfo)
{
}

void
Callbacks::closingConnection (MSN::Connection *conn)
{
    printf ("%s\n", __func__);
}

void
Callbacks::changedStatus (MSN::NotificationServerConnection *conn,
                          MSN::BuddyStatus state)
{
}

size_t
Callbacks::getDataFromSocket (void *sock,
                              char *data,
                              size_t size)
{
    printf ("%s\n", __func__);
    read (fd, data, size);
}

size_t
Callbacks::writeDataToSocket (void *sock,
                              char *data,
                              size_t size)
{
    printf ("%s\n", __func__);
    write (fd, data, size);
}

void *
Callbacks::connectToServer (std::string hostname,
                            int port,
                            bool *connected,
                            bool isSSL)
{
    printf ("%s: hostname=%s, port=%i, connected=%i, isSSL=%i\n", __func__,
            hostname.c_str(), port, *connected, isSSL);

    if (!isSSL)
    {
        pecan_socket_connect (hostname.c_str(), port, connect_cb, this);
        *connected = TRUE;
    }
}

int
Callbacks::listenOnPort (int port)
{
    printf ("%s\n", __func__);
}

std::string
Callbacks::getOurIP (void)
{
    printf ("%s\n", __func__);
}

void
Callbacks::log (int i,
                const char *s)
{
    printf ("%s: %i, %s\n", __func__, i, s);
}

int
Callbacks::getSocketFileDescriptor (void *sock)
{
    printf ("%s\n", __func__);
}

std::string
Callbacks::getSecureHTTPProxy ()
{
    printf ("%s\n", __func__);
    return "";
}

void
Callbacks::askFileTransfer (MSN::SwitchboardServerConnection *conn,
                            MSN::fileTransferInvite ft)
{
}

void
Callbacks::addedContactToGroup (MSN::NotificationServerConnection *conn,
                                bool added, std::string groupId,
                                std::string contactId)
{
}

void
Callbacks::removedContactFromGroup (MSN::NotificationServerConnection *conn,
                                    bool removed,
                                    std::string groupId,
                                    std::string contactId)
{
}

void
Callbacks::addedContactToAddressBook (MSN::NotificationServerConnection *conn,
                                      bool added,
                                      std::string passport,
                                      std::string displayName,
                                      std::string guid)
{
}

void
Callbacks::removedContactFromAddressBook (MSN::NotificationServerConnection *conn,
                                          bool removed,
                                          std::string contactId,
                                          std::string passport)
{
}

void
Callbacks::enabledContactOnAddressBook (MSN::NotificationServerConnection *conn,
                                        bool enabled,
                                        std::string contactId,
                                        std::string passport)
{
}

void
Callbacks::disabledContactOnAddressBook (MSN::NotificationServerConnection *conn,
                                         bool disabled,
                                         std::string contactId)
{
}

void
Callbacks::gotVoiceClipFile (MSN::SwitchboardServerConnection *conn,
                             unsigned int sessionID,
                             std::string file)
{
}

void
Callbacks::gotEmoticonFile (MSN::SwitchboardServerConnection *conn,
                            unsigned int sessionID,
                            std::string alias,
                            std::string file)
{
}

void
Callbacks::gotWinkFile (MSN::SwitchboardServerConnection *conn,
                        unsigned int sessionID,
                        std::string file)
{
}
