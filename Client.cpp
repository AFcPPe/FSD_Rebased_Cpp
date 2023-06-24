//
// Created by HarryXiong on 2023/6/23.
//

#include "Client.h"

Client::Client(QTcpSocket *s) {
    m_partialPacket = "";
    this->socket = s;
    connect(s, &QTcpSocket::readyRead, this, &Client::onIncomingData);
    connect(this,&Client::RaiseAddATCReceived,this,&Client::onAddATCReceived);
    this->clientStatus = Connected;
}

void Client::onIncomingData() {
    QString data = this->socket->readAll();
    processData(data);
}

void Client::processData(QString data) {
    if (data.length() == 0) return;
    if (data.right(data.length() - 1) == "\0") {
        data = data.left(data.length() - 1);
    }
    data = m_partialPacket + data;
    m_partialPacket = "";
    QStringList packets = data.split(PDUBase::PacketDelimeter);
    int topIndex = packets.length() - 1;
    if (packets[topIndex].length() > 0) {
        m_partialPacket = packets[topIndex];
        packets[topIndex] = "";
    }

    for (const auto &packet: packets) {
        if (packet.length() == 0) continue;
        QStringList fields = packet.split(PDUBase::Delimeter);
        const QChar prefixChar = fields[0][0];

        if (prefixChar == '@') {
            fields[0] = fields[0].mid(1);
            emit RaisePilotPositionReceived(PDUPilotPosition::fromTokens(fields));
        } else if (prefixChar == '^') {
            fields[0] = fields[0].mid(1);
            emit RaiseFastPilotPositionReceived(PDUFastPilotPosition::fromTokens(FastPilotPositionType::Fast, fields));
        } else if (prefixChar == '%') {
            fields[0] = fields[0].mid(1);
            emit RaiseATCPositionReceived(PDUATCPosition::fromTokens(fields));
        } else if (prefixChar == '#' || prefixChar == '$') {
            if (fields[0].length() < 3) {
                throw new PDUFormatException("Invalid PDU type.", packet);
            }
            QString pduTypeId = fields[0].mid(0, 3);
            fields[0] = fields[0].mid(3);
            if (pduTypeId == "#AA") {
                emit RaiseAddATCReceived(PDUAddATC::fromTokens(fields));
            } else if (pduTypeId == "#DA") {
                emit RaiseDeleteATCReceived(PDUDeleteATC::fromTokens(fields));
            } else if (pduTypeId == "#AP") {
                emit RaiseAddPilotReceived(PDUAddPilot::fromTokens(fields));
            } else if (pduTypeId == "#DP") {
                emit RaiseDeletePilotReceived(PDUDeletePilot::fromTokens(fields));
            } else if (pduTypeId == "#TM") {
                emit RaiseTextMessageReceived(PDUTextMessage::fromTokens(fields));
            } else if (pduTypeId == "$AR") {
                emit RaiseMetarResponseReceived(PDUMetarResponse::fromTokens(fields));
            } else if (pduTypeId == "#SB") {
                if (fields.length() >= 3) {
                    if (fields[2] == "PIR") {
                        emit RaisePlaneInfoRequestReceived(PDUPlaneInfoRequest::fromTokens(fields));
                    } else if (fields[2] == "PI" && fields.length() >= 4) {
                        if (fields[3] == "GEN") {
                            emit RaisePlaneInfoResponseReceived(PDUPlaneInfoResponse::fromTokens(fields));
                        }
                    }
                }
            } else if (pduTypeId == "$PI") {
                emit RaisePingReceived(PDUPing::fromTokens(fields));
            } else if (pduTypeId == "$PO") {
                emit RaisePongReceived(PDUPong::fromTokens(fields));
            } else if (pduTypeId == "$CQ") {
                emit RaiseClientQueryReceived(PDUClientQuery::fromTokens(fields));
            } else if (pduTypeId == "$CR") {
                emit RaiseClientQueryResponseReceived(PDUClientQueryResponse::fromTokens(fields));
            } else if (pduTypeId == "$!!") {
                emit RaiseKillRequestReceived(PDUKillRequest::fromTokens(fields));
            } else if (pduTypeId == "$ER") {
                emit RaiseProtocolErrorReceived(PDUProtocolError::fromTokens(fields));
            } else if (pduTypeId == "$SF") {
                emit RaiseSendFastReceived(PDUSendFast::fromTokens(fields));
            } else if (pduTypeId == "#SL") {
                emit RaiseFastPilotPositionReceived(
                        PDUFastPilotPosition::fromTokens(FastPilotPositionType::Slow, fields));
            } else if (pduTypeId == "#ST") {
                emit RaiseFastPilotPositionReceived(
                        PDUFastPilotPosition::fromTokens(FastPilotPositionType::Stopped, fields));
            }
        }
    }
}

void Client::onAddATCReceived(PDUAddATC pdu) {
    UserInfo info = Global::get().mysql->getUserInfo(pdu.CID);
    if(info.cid==""){
        qDebug()<<"User not in database";
        //TODO: 不存在用户的处理
        return;
    }
    if(info.encryptedPassword != pdu.Password){
        qDebug()<<"Incorrect Password";
        //TODO: 密码错误的处理
        return;
    }
    qDebug()<<"Correct";
    //TODO: 密码正确的处理
}
