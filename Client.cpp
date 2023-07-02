//
// Created by HarryXiong on 2023/6/23.
//

#include "Client.h"

Client::Client(QTcpSocket *s) {

    m_partialPacket = "";
    this->socket = s;
    //准备好读取数据
    connect(s, &QTcpSocket::readyRead, this, &Client::onIncomingData);
    connect(this,&Client::RaiseErrorToSend,this,&Client::showError);
    connect(this,&Client::RaiseMotdToRead,this,&Client::readMotd);
    connect(this,&Client::RaiseMetarRequestReceived,this,&Client::onMetarRequestReceived);
    //主动断开socket
    connect(s,&QAbstractSocket::disconnected,this,[&]{
        if(this->clientStatus== Connected){
            qInfo()<<"User disconnected from server. Callsign: unknown";
        }else{
            qInfo()<<qPrintable("User disconnected from server. Callsign: " + this->callsign);
        }
        this->bIsAlive = false;
        this->clientStatus = PendingKick;
        RaiseClientPendingKick(this);
    });
    this->clientStatus = Connected;
}

void Client::onIncomingData() {
    bIsAlive = true;
    QString data = this->socket->readAll();
    QtConcurrent::run(&Client::processData,this,data);
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
        if(this->clientStatus==Connected&&packet.length()>=3&&packet.left(3)!="#AA"&&packet.left(3)!="#AP"){
            m_partialPacket = packet+PDUBase::PacketDelimeter+m_partialPacket;
            continue;
        }
        QStringList fields = packet.split(PDUBase::Delimeter);
        const QChar prefixChar = fields[0][0];

        if (prefixChar == '@') {
            fields[0] = fields[0].mid(1);
            QtConcurrent::run(&Client::onPilotPositionReceived,this,PDUPilotPosition::fromTokens(fields));
            emit RaisePilotPositionReceived(PDUPilotPosition::fromTokens(fields));
        } else if (prefixChar == '^') {
            fields[0] = fields[0].mid(1);
            emit RaiseFastPilotPositionReceived(PDUFastPilotPosition::fromTokens(FastPilotPositionType::Fast, fields));
        } else if (prefixChar == '%') {
            fields[0] = fields[0].mid(1);
            QtConcurrent::run(&Client::onATCPositionReceived,this,PDUATCPosition::fromTokens(fields));
            emit RaiseATCPositionReceived(PDUATCPosition::fromTokens(fields));
        } else if (prefixChar == '#' || prefixChar == '$') {
            if (fields[0].length() < 3) {
                throw new PDUFormatException("Invalid PDU type.", packet);
            }
            QString pduTypeId = fields[0].mid(0, 3);
            fields[0] = fields[0].mid(3);
            if (pduTypeId == "#AA") {
                QtConcurrent::run(&Client::onAddATCReceived,this,PDUAddATC::fromTokens(fields));
                emit RaiseAddATCReceived(PDUAddATC::fromTokens(fields));
            } else if (pduTypeId == "#DA") {
                emit RaiseDeleteATCReceived(PDUDeleteATC::fromTokens(fields));
            } else if (pduTypeId == "#AP") {
                QtConcurrent::run(&Client::onAddPilotReceived,this,PDUAddPilot::fromTokens(fields));
                emit RaiseAddPilotReceived(PDUAddPilot::fromTokens(fields));
            } else if (pduTypeId == "#DP") {
                emit RaiseDeletePilotReceived(PDUDeletePilot::fromTokens(fields));
            } else if (pduTypeId == "#TM") {
                QtConcurrent::run(&Client::onTextMessageReceived,this,PDUTextMessage::fromTokens(fields));

                emit RaiseTextMessageReceived(PDUTextMessage::fromTokens(fields));
            } else if (pduTypeId == "$AR") {
                emit RaiseMetarResponseReceived(PDUMetarResponse::fromTokens(fields));
            } else if (pduTypeId == "#SB") {
                if (fields.length() >= 3) {
                    if (fields[2] == "PIR") {
                        QtConcurrent::run(&Client::onPlaneInfoRequestReceived,this,PDUPlaneInfoRequest::fromTokens(fields));
                        emit RaisePlaneInfoRequestReceived(PDUPlaneInfoRequest::fromTokens(fields));
                    } else if (fields[2] == "PI" && fields.length() >= 4) {
                        if (fields[3] == "GEN") {
                            QtConcurrent::run(&Client::onPlaneInfoResponseReceived,this,PDUPlaneInfoResponse::fromTokens(fields));
                            emit RaisePlaneInfoResponseReceived(PDUPlaneInfoResponse::fromTokens(fields));
                        }
                    }
                }
            } else if (pduTypeId == "$FP") {
                emit RaiseFlightPlanReceived(PDUFlightPlan::fromTokens(fields));
                QtConcurrent::run(&Client::onFlightPlanReceived,this,PDUFlightPlan::fromTokens(fields));
            } else if (pduTypeId == "$PI") {
                emit RaisePingReceived(PDUPing::fromTokens(fields));
            } else if (pduTypeId == "$AX") {

                emit RaiseMetarRequestReceived(PDUMetarRequest::fromTokens(fields));
            } else if (pduTypeId == "$PO") {
                emit RaisePongReceived(PDUPong::fromTokens(fields));
            } else if (pduTypeId == "$CQ") {
                QtConcurrent::run(&Client::onClientQueryReceived,this,PDUClientQuery::fromTokens(fields));
                emit RaiseClientQueryReceived(PDUClientQuery::fromTokens(fields));
            } else if (pduTypeId == "$CR") {
                QtConcurrent::run(&Client::onClientQueryResponseReceived,this,PDUClientQueryResponse::fromTokens(fields));
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
    emit RaiseUserAuthRequest(pdu.CID,pdu.Password,pdu.Rating, this);
    this->clientType = ATC;
    this->callsign = pdu.From;
    this->realName = pdu.RealName;
    this->cid = pdu.CID;
    this->rating = pdu.Rating;
}

void Client::showError(PDUProtocolError pdu) {
    this->socket->write(Serialize(pdu).toLocal8Bit());
    if(pdu.Fatal){
        QtConcurrent::run([&]{
            QThread::sleep(1);
            this->bIsAlive = false;
            this->clientStatus = PendingKick;
            emit RaiseClientPendingKick(this);
        });

    }
}

void Client::readMotd() {
//    QThread::sleep(3);
//    this->socket->write(QString("$SFSERVER:"+this->callsign+":1\r\n").toLocal8Bit());
    for(const auto& line:Global::get().s.qlsMotd){
        this->socket->write(Serialize(PDUTextMessage("server",this->callsign , line)).toLocal8Bit());
    }
}

void Client::onAddPilotReceived(PDUAddPilot pdu) {
    emit RaiseUserAuthRequest(pdu.CID,pdu.Password,pdu.Rating,this);
    this->clientType = Pilot;
    this->callsign = pdu.From;
    this->realName = pdu.RealName;
    this->cid = pdu.CID;
    this->rating = pdu.Rating;
    //TODO: 飞行计划的下载
}

void Client::onPilotPositionReceived(PDUPilotPosition pdu){
    if(this->clientStatus==Connected)return;
    this->location.lon = pdu.Lon;
    this->location.lat = pdu.Lat;
    this->squawkCode = pdu.SquawkCode;
    this->squawkModeC = pdu.SquawkingModeC;
    this->identing = pdu.Identing;
    this->trueAltitude = pdu.TrueAltitude;
    this->pressureAltitude = pdu.PressureAltitude;
    this->pitch = pdu.Pitch;
    this->heading = pdu.Heading;
    this->bank = pdu.Bank;
    emit RaiseForwardInfo(this,"@", Serialize(pdu));
}

void Client::onATCPositionReceived(PDUATCPosition pdu) {
    if(this->clientStatus==Connected)return;
    this->location.lon = pdu.Lon;
    this->location.lat = pdu.Lat;
    this->facility = pdu.Facility;
    this->frequencies = pdu.Frequencies;
    this->visualRange = pdu.VisibilityRange;
    emit RaiseForwardInfo(this,"@", Serialize(pdu));
}

void Client::onFlightPlanReceived(PDUFlightPlan pdu) {
    flightPlan.flightRule = pdu.FlightRule;
    flightPlan.type = pdu.Type;
    flightPlan.tas = pdu.Tas;
    flightPlan.dep = pdu.Dep;
    flightPlan.depTime = pdu.DepTime;
    flightPlan.actualDepTime = pdu.ActualDepTime;
    flightPlan.cruiseAlt = pdu.CruiseAlt;
    flightPlan.dest = pdu.Dest;
    flightPlan.enrouteHour = pdu.EnrouteHour;
    flightPlan.enrouteMin = pdu.EnrouteMin;
    flightPlan.fobHour = pdu.FobHour;
    flightPlan.fobMin = pdu.FobMin;
    flightPlan.alterDest = pdu.AlterDest;
    flightPlan.remark = pdu.Remark;
    flightPlan.route = pdu.Route;
    pdu.To = "*A";
    emit RaiseForwardInfo(this,"*A", Serialize(pdu));
}

void Client::onClientQueryReceived(PDUClientQuery pdu) {
    if(pdu.To.toLower() == "server"){
        emit RaiseQueryToResponse(this,pdu);
        return;
    }
    if(pdu.To.left(1)=="@"){
        emit RaiseForwardInfo(this,"**", Serialize(pdu));
    }else{
        emit RaiseForwardInfo(this,pdu.To, Serialize(pdu));
    }

}

void Client::onClientQueryResponseReceived(PDUClientQueryResponse pdu) {
    emit RaiseForwardInfo(this,pdu.To, Serialize(pdu));
}

void Client::onMetarRequestReceived(PDUMetarRequest pdu) {
    QString metar = Global::get().weather->getMetar(pdu.Station);
    socket->write(Serialize(PDUMetarResponse(this->callsign,metar)).toLocal8Bit());
}

void Client::onPlaneInfoRequestReceived(PDUPlaneInfoRequest pdu) {
    emit RaiseForwardInfo(this,pdu.To, Serialize(pdu));
}

void Client::onPlaneInfoResponseReceived(PDUPlaneInfoResponse pdu) {
    emit RaiseForwardInfo(this,pdu.To, Serialize(pdu));
}

void Client::onTextMessageReceived(PDUTextMessage pdu) {
    if(pdu.To == "*"){
        if(this->rating<NetworkRating::SUP){
            emit RaiseErrorToSend(PDUProtocolError("SERVER", this->callsign, NetworkError::InvalidPositionForRating, cid, "Unauthorized", false));
        }
    }
    if(pdu.To.left(1)=="@") {
        emit RaiseForwardInfo(this, "*", Serialize(pdu));
        return;
    }
    emit RaiseForwardInfo(this,pdu.To, Serialize(pdu));

}
