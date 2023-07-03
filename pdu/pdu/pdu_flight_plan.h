

#ifndef PDU_FLIGHT_PLAN_H
#define PDU_FLIGHT_PLAN_H

#include "pdu_base.h"

#include <QString>
#include <QStringList>
#include <QVector>

class PDUFlightPlan : public PDUBase
{
public:
    PDUFlightPlan(QString callsign, QString to, FlightRules flightRule, QString type, int tas, QString dep, int depTime, int actualDepTime, QString cruiseAlt, QString dest, int enrouteHour,int enrouteMin, int fobHour,int fobMin,QString alterDest,QString remark, QString route);

    QStringList toTokens() const;

    static PDUFlightPlan fromTokens(const QStringList& fields);

    static QString pdu() { return "$FP"; }

    QString Callsign;
    FlightRules FlightRule;
    QString Type;
    int Tas;
    QString Dep;
    int DepTime;
    int ActualDepTime;
    QString CruiseAlt;
    QString Dest;
    int EnrouteHour;
    int EnrouteMin;
    int FobHour;
    int FobMin;
    QString AlterDest;
    QString Remark;
    QString Route;
private:
    PDUFlightPlan();
};

#endif
