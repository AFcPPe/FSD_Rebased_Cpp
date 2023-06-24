//
// Created by HarryXiong on 2023/6/23.
//

#include "Server.h"
#include "Global.h"

Server::Server() {
    m_server = new QTcpServer;
    m_server->listen(QHostAddress::Any,Global::get().s.usServerPort);
    connect(m_server,&QTcpServer::newConnection,this,&Server::onNewConnection);
}



void Server::onNewConnection() {
    if(m_server->hasPendingConnections()){
        QTcpSocket* socket =  m_server->nextPendingConnection();
        qlClientPool.append(new Client(socket));
        qInfo()<<QString("New connection from %1 established").arg(socket->peerAddress().toString()).toStdString().c_str();
    }
}