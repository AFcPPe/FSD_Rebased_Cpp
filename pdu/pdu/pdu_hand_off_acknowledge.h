

#ifndef PDU_HANDOFF_ACKNOWLEDGE_H
#define PDU_HANDOFF_ACKNOWLEDGE_H

#include "pdu_base.h"

#include <QString>
#include <QStringList>
#include <QVector>

class PDUHandOffAcknowledge : public PDUBase
{
public:
    PDUHandOffAcknowledge(QString callsign,QString target,QString tCallsign);
    QStringList toTokens() const;
    static PDUHandOffAcknowledge fromTokens(const QStringList& fields);
    static QString pdu() { return "$HA"; }
    QString tCallsign;

private:
    PDUHandOffAcknowledge();
};

#endif
