

#ifndef PDU_PC_H
#define PDU_PC_H

#include <QString>
#include "pdu_base.h"

class PDUPC : public PDUBase
{
public:
    PDUPC(QString from, QString to, QStringList payload = {});

    QStringList toTokens() const;

    static PDUPC fromTokens(const QStringList& fields);

    static QString pdu() { return "#PC"; }
    QStringList Payload;

private:
    PDUPC();
};

#endif
