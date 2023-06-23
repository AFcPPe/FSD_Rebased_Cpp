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
    QList<Client*> clientPool;
private slots:
    void onNewConnection();
};


#endif //FSD_REBASED_SERVER_H
