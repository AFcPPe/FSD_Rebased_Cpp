//
// Created by HarryXiong on 2023/6/23.
//

#ifndef FSD_REBASED_CLIENT_H
#define FSD_REBASED_CLIENT_H


#include <QObject>
#include <QTcpSocket>
#include "pdu/pdu/pdu_base.h"
#include "pdu/pdu/pdu_add_atc.h"
#include "pdu/pdu/pdu_atc_position.h"
#include "pdu/pdu/pdu_delete_atc.h"
#include "pdu/pdu/pdu_add_pilot.h"
#include "pdu/pdu/pdu_delete_pilot.h"
#include "pdu/pdu/pdu_broadcast_message.h"
#include "pdu/pdu/pdu_wallop.h"
#include "pdu/pdu/pdu_radio_message.h"
#include "pdu/pdu/pdu_text_message.h"
#include "pdu/pdu/pdu_plane_info_request.h"
#include "pdu/pdu/pdu_plane_info_response.h"
#include "pdu/pdu/pdu_ping.h"
#include "pdu/pdu/pdu_pong.h"
#include "pdu/pdu/pdu_client_query.h"
#include "pdu/pdu/pdu_client_query_response.h"
#include "pdu/pdu/pdu_kill_request.h"
#include "pdu/pdu/pdu_protocol_error.h"
#include "pdu/pdu/pdu_fast_pilot_position.h"
#include "pdu/pdu/pdu_pilot_position.h"
#include "pdu/pdu/pdu_metar_response.h"
#include "pdu/pdu/pdu_metar_request.h"
#include "pdu/pdu/pdu_send_fast.h"
#include "Global.h"
#include "Mysql.h"

enum ClientStatus{
    Connected,
    Logon,
    Inactive,
    PendingKick
};

enum ClientType{
    Pilot,
    ATC
};

class Client: public QObject{
Q_OBJECT
public:
    explicit Client(QTcpSocket *s);
    ClientStatus clientStatus;
    ClientType clientType;
    QTcpSocket *socket;
    bool bIsAlive;
    QString m_partialPacket;
signals:
    void RaisePilotPositionReceived(PDUPilotPosition pdu);
    void RaiseFastPilotPositionReceived(PDUFastPilotPosition pdu);
    void RaiseATCPositionReceived(PDUATCPosition pdu);
    void RaiseAddATCReceived(PDUAddATC pdu);
    void RaiseDeleteATCReceived(PDUDeleteATC pdu);
    void RaiseAddPilotReceived(PDUAddPilot pdu);
    void RaiseDeletePilotReceived(PDUDeletePilot pdu);
    void RaiseBroadcastMessageReceived(PDUBroadcastMessage pdu);
    void RaiseWallopReceived(PDUWallop pdu);
    void RaiseRadioMessageReceived(PDURadioMessage pdu);
    void RaiseTextMessageReceived(PDUTextMessage pdu);
    void RaiseMetarResponseReceived(PDUMetarResponse pdu);
    void RaisePlaneInfoRequestReceived(PDUPlaneInfoRequest pdu);
    void RaisePlaneInfoResponseReceived(PDUPlaneInfoResponse pdu);
    void RaisePingReceived(PDUPing pdu);
    void RaisePongReceived(PDUPong pdu);
    void RaiseClientQueryReceived(PDUClientQuery pdu);
    void RaiseClientQueryResponseReceived(PDUClientQueryResponse pdu);
    void RaiseKillRequestReceived(PDUKillRequest pdu);
    void RaiseProtocolErrorReceived(PDUProtocolError pdu);
    void RaiseSendFastReceived(PDUSendFast pdu);
private:
    void processData(QString data);
private slots:
    void onIncomingData();

void onAddATCReceived(PDUAddATC pdu);



};


#endif //FSD_REBASED_CLIENT_H