#include "pdu_flight_plan.h"

PDUFlightPlan::PDUFlightPlan() : PDUBase() {}

PDUFlightPlan::PDUFlightPlan(QString callsign, QString to, FlightRules flightRule, QString type, int tas, QString dep,
                             int depTime, int actualDepTime, QString cruiseAlt, QString dest, int enrouteHour,
                             int enrouteMin, int fobHour,int fobMin,QString alterDest,QString remark, QString route)
 : PDUBase(callsign, to)
{
    Callsign = callsign;
    To = to;
    FlightRule = flightRule;
    Type = type;
    Tas = tas;
    Dep = dep;
    DepTime = depTime;
    ActualDepTime = actualDepTime;
    CruiseAlt = cruiseAlt;
    Dest = dest;
    EnrouteHour = enrouteHour;
    EnrouteMin = enrouteMin;
    FobHour = fobHour;
    FobMin = fobMin;
    AlterDest = alterDest;
    Remark = remark;
    Route = route;
}

QStringList PDUFlightPlan::toTokens() const
{
    QStringList tokens;
    tokens.append(From);
    tokens.append(To);
    tokens.append(toQString(FlightRule));
    tokens.append(Type);
    tokens.append(QString::number(Tas));
    tokens.append(Dep);
    tokens.append(QString::number(DepTime));
    tokens.append(QString::number(ActualDepTime));
    tokens.append(CruiseAlt);
    tokens.append(Dest);
    tokens.append(QString::number(EnrouteHour));
    tokens.append(QString::number(EnrouteMin));
    tokens.append(QString::number(FobHour));
    tokens.append(QString::number(FobMin));
    tokens.append(AlterDest);
    tokens.append(Remark);
    tokens.append(Route);
    return tokens;
}

PDUFlightPlan PDUFlightPlan::fromTokens(const QStringList &tokens)
{
    if(tokens.size() < 17) {
        throw PDUFormatException("Invalid field count.", Reassemble(tokens));
    }

    return PDUFlightPlan(tokens[0],tokens[1],fromQString<FlightRules>(tokens[2]),tokens[3],tokens[4].toInt(),tokens[5],tokens[6].toInt(),tokens[7].toInt(),tokens[8],
                         tokens[9],tokens[10].toInt(),tokens[11].toInt(),tokens[12].toInt(),tokens[13].toInt(),tokens[14],tokens[15],tokens[16]);
}
