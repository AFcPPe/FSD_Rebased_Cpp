//
// Created by HarryXiong on 2023/6/23.
//

#include "Server.h"
#include "Global.h"

double rad(double d)
{
    return d * pi /180.0;
}

Server::Server() {

    m_server = new QTcpServer;
    m_server->listen(QHostAddress::Any,Global::get().s.usServerPort);
    connect(m_server,&QTcpServer::newConnection,this,&Server::onNewConnection);
    qtStatus.setInterval(Global::get().s.status_check_time*1000);
    qtStatus.start();
    connect(&qtStatus,&QTimer::timeout,this,&Server::onCheckStatus);
    qInfo()<<"Server listening on "<<Global::get().s.usServerPort;
}

void Server::onNewConnection() {
    if(m_server->hasPendingConnections()){
        QTcpSocket* socket =  m_server->nextPendingConnection();
        Client *newClient = new Client(socket);
        qlClientPool.append(newClient);
        connect(newClient,&Client::RaiseClientPendingKick,this,&Server::onUserPendingKick);
        connect(newClient,&Client::RaiseForwardInfo,this,&Server::onForwardInfoRequest);
        connect(newClient,&Client::RaiseQueryToResponse,this,&Server::onQueryToReqsonse);
        qInfo()<<qPrintable(QString("New connection from %1 established").arg(socket->peerAddress().toString()));
    }
}

void Server::onUserPendingKick(Client* client) {
    int index = qlClientPool.indexOf(client);
    if(index!=-1){
        qInfo()<<qPrintable(QString("Connection is destroyed by the server. IP: %1").arg(qlClientPool.value(index)->socket->peerAddress().toString()));
        qlClientPool.value(index)->socket->deleteLater();
        qlClientPool.removeAt(index);
        if(client->clientStatus!=Connected){
            if(client->clientType==Pilot){
                onForwardInfoRequest(client,"@", Serialize(PDUDeletePilot(client->callsign,client->cid)));
            } else
            if(client->clientType==ATC){
                onForwardInfoRequest(client,"@", Serialize(PDUDeleteATC(client->callsign,client->cid)));
            }
        }
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

double Server::calculateDistanceBetweenClients(Client * A, Client * B) {
    double a;
    double b;
    double radLat1 = rad(A->location.lat);
    double radLat2 = rad(B->location.lat);
    a = radLat1 - radLat2;
    b = rad(A->location.lon) - rad(A->location.lon);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = s/1.852;
    return s;
}

void Server::onForwardInfoRequest(Client* from,QString to, QString Packet) {
    if(to=="*"){
        for(auto client:qlClientPool){
            if(client == from)continue;
            if(client->clientStatus== Connected||client->clientStatus== PendingKick)continue;
            client->socket->write(Packet.arg(client->callsign).toLocal8Bit());
        }
        return;
    }
    if(to=="@"){
        for(auto client:qlClientPool){
            if(client == from)continue;
            if(client->clientStatus== Connected||client->clientStatus== PendingKick)continue;

            client->socket->write(Packet.toLocal8Bit());
        }
        return;
    }
    if(to=="*A"){
        for(auto client:qlClientPool){
            if(client == from)continue;
            if(client->clientStatus== Connected||client->clientStatus== PendingKick)continue;
            if(client->clientType == Pilot) continue;
            client->socket->write(Packet.toLocal8Bit());
        }
        return;
    }
    if(to=="*P"){
        for(auto client:qlClientPool){
            if(client == from)continue;
            if(client->clientStatus== Connected||client->clientStatus== PendingKick)continue;
            if(client->clientType == ATC) continue;
            client->socket->write(Packet.toLocal8Bit());
        }
        return;
    }
}

void Server::onQueryToReqsonse(Client *from, PDUClientQuery pdu) {
    if(pdu.QueryType == ClientQueryType::FlightPlan){
        for(auto client: qlClientPool){
            if(client->callsign == pdu.Payload[0]){
                qDebug()<<Serialize(PDUFlightPlan(client->callsign, from->callsign, client->flightPlan.flightRule, client->flightPlan.type, client->flightPlan.tas, client->flightPlan.dep,
                                                  client->flightPlan.depTime, client->flightPlan.actualDepTime, client->flightPlan.cruiseAlt, client->flightPlan.dest, client->flightPlan.enrouteHour,
                                                  client->flightPlan.enrouteMin, client->flightPlan.fobHour,client->flightPlan.fobMin,client->flightPlan.alterDest,client->flightPlan.remark, client->flightPlan.route));
                from->socket->write(Serialize(PDUFlightPlan(client->callsign, from->callsign, client->flightPlan.flightRule, client->flightPlan.type, client->flightPlan.tas, client->flightPlan.dep,
                                                            client->flightPlan.depTime, client->flightPlan.actualDepTime, client->flightPlan.cruiseAlt, client->flightPlan.dest, client->flightPlan.enrouteHour,
                                                            client->flightPlan.enrouteMin, client->flightPlan.fobHour,client->flightPlan.fobMin,client->flightPlan.alterDest,client->flightPlan.remark, client->flightPlan.route)).toLocal8Bit());
                return;
            }
        }
    }
}
