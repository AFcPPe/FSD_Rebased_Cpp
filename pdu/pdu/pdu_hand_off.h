

#ifndef PDU_HANDOFF_H
#define PDU_HANDOFF_H

#include "pdu_base.h"

#include <QString>
#include <QStringList>
#include <QVector>

class PDUHandOff : public PDUBase
{
public:
    PDUHandOff(QString callsign,QString target,QString Object);
    QStringList toTokens() const;
    static PDUHandOff fromTokens(const QStringList& fields);
    static QString pdu() { return "$HO"; }
    QString Object;

private:
    PDUHandOff();
};

#endif
