#ifndef CB_H
#define CB_H

#include <msn/msn.h>

class Callbacks:public MSN::Callbacks
{
    virtual void
    registerSocket (void *s,
                    int read,
                    int write,
                    bool isSSL);

    virtual void
    unregisterSocket (void *s);

    virtual void
    closeSocket (void *s);

    virtual void
    showError (MSN::Connection *conn,
               std::string msg);

    virtual void
    buddyChangedStatus (MSN::NotificationServerConnection *conn,
                        MSN::Passport buddy,
                        std::string friendlyname,
                        MSN::BuddyStatus state,
                        unsigned int clientID,
                        std::string msnobject);

    virtual void
    buddyOffline (MSN::NotificationServerConnection *conn,
                  MSN::Passport buddy);

    virtual void
    log (int writing,
         const char *buf);

    virtual void
    buddyChangedPersonalInfo (MSN::NotificationServerConnection *conn,
                              MSN::Passport fromPassport,
                              MSN::personalInfo);

    virtual void
    gotMessageSentACK (MSN::SwitchboardServerConnection *conn,
                       int trID);

    virtual void
    gotFriendlyName (MSN::NotificationServerConnection *conn,
                     std::string friendlyname);

    virtual void
    gotBuddyListInfo (MSN::NotificationServerConnection *conn,
                      MSN::ListSyncInfo *data);

    virtual void
    gotLatestListSerial (MSN::NotificationServerConnection *conn,
                         std::string lastChange);

    virtual void
    gotGTC (MSN::NotificationServerConnection *conn, char c);

    virtual void
    gotBLP (MSN::NotificationServerConnection *conn, char c);

    virtual void
    gotContactDisplayPicture (MSN::SwitchboardServerConnection*,
                              MSN::Passport, std::string);

    virtual void
    addedListEntry (MSN::NotificationServerConnection *conn,
                    MSN::ContactList list,
                    MSN::Passport buddy,
                    std::string friendlyname);

    virtual void
    removedListEntry (MSN::NotificationServerConnection *conn,
                      MSN::ContactList list,
                      MSN::Passport buddy);

    virtual void
    addedGroup (MSN::NotificationServerConnection *conn,
                bool added,
                std::string groupName,
                std::string groupID);

    virtual void
    removedGroup (MSN::NotificationServerConnection *conn,
                  bool removed,
                  std::string groupID);

    virtual void
    renamedGroup (MSN::NotificationServerConnection *conn,
                  bool renamed,
                  std::string newGroupName,
                  std::string groupID);

    virtual void
    addedContactToGroup (MSN::NotificationServerConnection *conn,
                         bool added,
                         std::string groupId,
                         std::string contactId);

    virtual void
    removedContactFromGroup (MSN::NotificationServerConnection *conn,
                             bool removed,
                             std::string groupId,
                             std::string contactId);

    virtual void
    addedContactToAddressBook (MSN::NotificationServerConnection *conn,
                               bool added,
                               std::string passport,
                               std::string displayName,
                               std::string guid);

    virtual void
    removedContactFromAddressBook (MSN::NotificationServerConnection *conn,
                                   bool removed,
                                   std::string contactId,
                                   std::string passport);

    virtual void
    fileTransferInviteResponse (MSN::SwitchboardServerConnection *conn,
                                unsigned int sessionID,
                                bool response);

    virtual void
    enabledContactOnAddressBook (MSN::NotificationServerConnection *conn,
                                 bool enabled,
                                 std::string contactId,
                                 std::string passport);

    virtual void
    disabledContactOnAddressBook (MSN::NotificationServerConnection *conn,
                                  bool disabled,
                                  std::string contactId);

    virtual void
    gotSwitchboard (MSN::SwitchboardServerConnection *conn,
                    const void *tag);

    virtual void
    buddyJoinedConversation (MSN::SwitchboardServerConnection *conn,
                             MSN::Passport buddy,
                             std::string friendlyname,
                             int is_initial);

    virtual void
    buddyLeftConversation (MSN::SwitchboardServerConnection *conn,
                           MSN::Passport buddy);

    virtual void
    gotInstantMessage (MSN::SwitchboardServerConnection *conn,
                       MSN::Passport buddy,
                       std::string friendlyname,
                       MSN::Message *msg);

    virtual void
    gotEmoticonNotification (MSN::SwitchboardServerConnection *conn,
                             MSN::Passport buddy,
                             std::string alias,
                             std::string msnobject);

    virtual void
    failedSendingMessage (MSN::Connection *conn);

    virtual void
    buddyTyping (MSN::SwitchboardServerConnection *conn,
                 MSN::Passport buddy,
                 std::string friendlyname);

    virtual void
    gotNudge (MSN::SwitchboardServerConnection *conn,
              MSN::Passport from);

    virtual void
    gotVoiceClipNotification (MSN::SwitchboardServerConnection *conn,
                              MSN::Passport from,
                              std::string msnobject);

    virtual void
    gotWinkNotification (MSN::SwitchboardServerConnection *conn,
                         MSN::Passport from,
                         std::string msnobject);

    virtual void
    gotInk (MSN::SwitchboardServerConnection *conn,
            MSN::Passport from,
            std::string image);

    virtual void
    gotActionMessage (MSN::SwitchboardServerConnection *conn,
                      MSN::Passport username,
                      std::string message);

    virtual void
    gotInitialEmailNotification (MSN::NotificationServerConnection *conn,
                                 int msgs_inbox,
                                 int unread_inbox,
                                 int msgs_folders,
                                 int unread_folders);

    virtual void
    gotNewEmailNotification (MSN::NotificationServerConnection *conn,
                             std::string from,
                             std::string subject);

    virtual void
    fileTransferProgress (MSN::SwitchboardServerConnection *conn,
                          unsigned int sessionID,
                          long long unsigned transferred,
                          long long unsigned total);

    virtual void
    fileTransferFailed (MSN::SwitchboardServerConnection *conn,
                        unsigned int sessionID,
                        MSN::fileTransferError error);

    virtual void
    fileTransferSucceeded (MSN::SwitchboardServerConnection *conn,
                           unsigned int sessionID);

    virtual void
    gotNewConnection (MSN::Connection *conn);

    virtual void
    gotOIMList (MSN::NotificationServerConnection *conn,
                std::vector < MSN::eachOIM > OIMs);

    virtual void
    gotOIM (MSN::NotificationServerConnection *conn,
            bool success,
            std::string id,
            std::string message);

    virtual void
    gotOIMSendConfirmation (MSN::NotificationServerConnection *conn,
                            bool success,
                            int id);

    virtual void
    gotOIMDeleteConfirmation (MSN::NotificationServerConnection *conn,
                              bool success,
                              std::string id);

    virtual void
    closingConnection (MSN::Connection *conn);

    virtual void
    changedStatus (MSN::NotificationServerConnection *conn,
                   MSN::BuddyStatus state);

    virtual void
    gotVoiceClipFile (MSN::SwitchboardServerConnection *conn,
                      unsigned int sessionID,
                      std::string file);

    virtual void
    gotEmoticonFile (MSN::SwitchboardServerConnection *conn,
                     unsigned int sessionID,
                     std::string alias,
                     std::string file);

    virtual void
    gotWinkFile (MSN::SwitchboardServerConnection *conn,
                 unsigned int sessionID,
                 std::string file);

    virtual void *
    connectToServer (std::string server,
                     int port,
                     bool *connected,
                     bool isSSL);

    virtual size_t
    getDataFromSocket (void *sock,
                       char *data,
                       size_t size);

    virtual size_t
    writeDataToSocket (void *sock,
                       char *data,
                       size_t size);

    virtual void
    connectionReady (MSN::Connection *conn);

    virtual void
    askFileTransfer (MSN::SwitchboardServerConnection *conn,
                     MSN::fileTransferInvite ft);

    virtual int
    listenOnPort (int port);

    virtual int
    getSocketFileDescriptor (void *sock);

    virtual std::string
    getOurIP ();

    virtual std::string
    getSecureHTTPProxy ();

    public:
    MSN::NotificationServerConnection *ns;
    int fd;
};

#endif /* CB_H */
