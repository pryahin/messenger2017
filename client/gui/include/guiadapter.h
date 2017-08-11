#ifndef GUIADAPTER_H
#define GUIADAPTER_H

#include <QObject>
#include <QVariant>
#include <QQmlApplicationEngine>

#include <unordered_map>


#include <include/models/chats_filter_proxy_model.h>
#include <include/models/contacts_model.h>
#include <include/models/messages_model.h>
#include <include/models/models_elements.h>

///заглушки со структурами данных ядра
#include <ctime>
namespace ebucheeYadro {
    class Message
    {
    public:
        std::string chat_id;
        std::string from_uuid;
        std::time_t sendTime;
        std::string text;
    };


    class Chat
    {
    public:
        std::string chat_id;
        std::string name;
        std::string avatar;
        Message lastMessage;
        int unreadable;
    };


    class Contact
    {
    public:
        std::string uuid;
        std::string name;
        std::string avatar;
    };
}


namespace m2 {
namespace gui {
namespace controler {


class GuiAdapter : public QObject
{
    Q_OBJECT

public:
    static GuiAdapter* getGuiAdapter();

public slots:
    void addModelsToEngineRoot(QQmlApplicationEngine* engine);

    ///geters-seters
    QString getMyUuid() const;
    void setMyUuidCallback(const std::string &value);

    QString getCurrentChatID() const;
    void changeCurrentChat(const QString &ID);

    MessagesModel* getMessagesModel() const;
    ChatsFilterProxyModel* getChatsModel() const;
    ContactsModel* getContactsModel() const;


    void getMyUuidCallback(std::string uuid);
    void connectionLostCallback();

    QStringList getServerList();

    void loginToServer();
    void loginToServerCallback();

    void registerToServer(const int &serverIndex);
    void registerToServerCallback();


    ///for messages
    void sendMessage(ModelsElements::MessageData) const;
    void loadChatHistory();
    void uploadChatHistory() const;


    ///fot messages callbacks
    void receiveMessageCallback(ebucheeYadro::Message mess);
    void loadChatHistoryCallback(std::vector<ebucheeYadro::Message>);
    void uploadChatHistoryCallback(std::vector<ebucheeYadro::Message>);



    ///for chats
    void deleteChat(const QString &chatID);
    void createChat(const QString &uuid);
    QHash<QString, ModelsElements::ChatData> loadChats();


    ///for chats callbacks
    void addChatCallback(ebucheeYadro::Chat chat);
    void loadChatsCallback(std::unordered_map <std::string, ebucheeYadro::Chat> chatsTable);


    ///for contacts
    QHash<QString, ModelsElements::ContactData> loadContacts();


    ///for contacts callbacks
    void loadContactsCallback(std::unordered_map <std::string, ebucheeYadro::Contact> contactsTable);


signals:
    void messagesLoaded(QVector<ModelsElements::MessageData>);
    void newMessage(ModelsElements::MessageData);

    void newChat(ModelsElements::ChatData);
    void currentChatChanged(const QString);

    void serverListLoaded(QStringList);
    void loginSuccessed();
    void loginFailed(const QString msg);

    void registrationSuccessed();
    void registrationFailed(const QString msg);


private:
    GuiAdapter();
    ~GuiAdapter();


    MessagesModel* messages;
    ChatsFilterProxyModel* chats;
    ContactsModel* contacts;

    QString myUuid;
    QString currentChatID;
};

}
}
}
#endif // GUIADAPTER_H
