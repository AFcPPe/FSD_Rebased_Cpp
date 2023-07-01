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
#include "pdu/pdu/pdu_flight_plan.h"
#include "Global.h"
#include "Mysql.h"
#include <QtConcurrent/QtConcurrent>
#include "Redis.h"

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

struct ClientLocation{
    double lat;
    double lon;
};

struct FlightPlan {
    FlightRules flightRule;
    QString type;
    int tas;
    QString dep;
    int depTime;
    int actualDepTime;
    QString cruiseAlt;
    QString dest;
    int enrouteHour;
    int enrouteMin;
    int fobHour;
    int fobMin;
    QString alterDest;
    QString remark;
    QString route;
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
    //ATC和Pilot的共有属性
    QString callsign;
    QString realName;
    QString cid;
    NetworkRating rating;
    ClientLocation location;
    int visualRange;

    //Pilot的专属属性
    int squawkCode;
    bool squawkModeC;
    bool identing;
    int trueAltitude;
    int pressureAltitude;
    double pitch;
    double heading;
    double bank;
    FlightPlan flightPlan;

    //ATC的专属属性
    NetworkFacility facility;
    QList<int> frequencies;

    //Pilot数据更新
    void updatePilotPos();
    void updatePilotData();
    void updatePilotFP();

    //ATC数据更新
    void updateATCPos();
    void updateATCData();


signals:
    //PDU Event
    void RaisePilotPositionReceived(PDUPilotPosition pdu);
    void RaiseFlightPlanReceived(PDUFlightPlan pdu);
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
    //OuterEvent
    void RaiseClientPendingKick(Client* client);
    void RaiseErrorToSend(PDUProtocolError pdu);
    void RaiseMotdToRead();
    void RaiseForwardInfo(Client* from,QString to,QString Packet);
    void RaiseQueryToResponse(Client* client, PDUClientQuery);
private:
    void processData(QString data);

    void onAddATCReceived(PDUAddATC pdu);
    void onAddPilotReceived(PDUAddPilot pdu);
    void onPilotPositionReceived(PDUPilotPosition pdu);
    void onATCPositionReceived(PDUATCPosition pdu);
    void onFlightPlanReceived(PDUFlightPlan pdu);
    void onClientQueryReceived(PDUClientQuery pdu);

    void uploadFlightPlan();
private slots:
    void onIncomingData();
    void showError(PDUProtocolError pdu);
    void readMotd();
};


#endif //FSD_REBASED_CLIENT_H