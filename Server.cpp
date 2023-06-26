//
// Created by HarryXiong on 2023/6/23.
//

#include "Server.h"
#include "Global.h"

Server::Server() {
    m_server = new QTcpServer;
    m_server->listen(QHostAddress::Any,Global::get().s.usServerPort);
    connect(m_server,&QTcpServer::newConnection,this,&Server::onNewConnection);
    qtStatus.setInterval(Global::get().s.status_check_time*1000);
    qtStatus.start();
    connect(&qtStatus,&QTimer::timeout,this,&Server::onCheckStatus);
}

void Server::onNewConnection() {
    if(m_server->hasPendingConnections()){
        QTcpSocket* socket =  m_server->nextPendingConnection();
        Client *newClient = new Client(socket);
        qlClientPool.append(newClient);
        connect(newClient,&Client::RaiseClientPendingKick,this,&Server::onUserPendingKick);
        qInfo()<<qPrintable(QString("New connection from %1 established").arg(socket->peerAddress().toString()));
    }
}

void Server::onUserPendingKick(Client* client) {

    int index = qlClientPool.indexOf(client);
    if(index!=-1){
        QThread::msleep(1000);
        qInfo()<<qPrintable(QString("Connection is destroyed by the server. IP: %1").arg(qlClientPool.value(index)->socket->peerAddress().toString()));
        qlClientPool.value(index)->socket->deleteLater();
        qlClientPool.removeAt(index);
        delete client;
    }
}

void Server::onCheckStatus() {
    for (auto client : qlClientPool){
        if(!client->bIsAlive){
            client->clientStatus = PendingKick;
            qInfo()<<"A user is kick from the server due inactive.";
        }
        if(client->clientStatus == PendingKick){
            onUserPendingKick(client);
        }
        client->bIsAlive = false;
    }
}
