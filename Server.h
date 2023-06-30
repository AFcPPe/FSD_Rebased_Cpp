//
// Created by HarryXiong on 2023/6/23.
//

#ifndef FSD_REBASED_SERVER_H
#define FSD_REBASED_SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include "Client.h"




class Server :public QObject{
    Q_OBJECT
public:
    Server();
    QTcpServer *m_server;
    QList<Client*> qlClientPool;
    QTimer qtStatus;
private:
    static double calculateDistanceBetweenClients(Client*,Client*);

private slots:
    void onNewConnection();
    void onUserPendingKick(Client *client);
    void onCheckStatus();
    void onForwardInfoRequest(Client* from,QString to,QString Packet);
    void onQueryToReqsonse(Client* from,PDUClientQuery pdu);
};


#endif //FSD_REBASED_SERVER_H
