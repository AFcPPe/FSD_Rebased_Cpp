

#include "pdu_pc.h"

PDUPC::PDUPC() : PDUBase() {}

PDUPC::PDUPC(QString from, QString to, QStringList payload) :
    PDUBase(from, to)
{
    Payload = payload;
}

QStringList PDUPC::toTokens() const
{
    QStringList tokens;
    tokens.append(From);
    tokens.append(To);
    tokens.append(Payload);
    return tokens;
}

PDUPC PDUPC::fromTokens(const QStringList &tokens)
{

    if(tokens.length() < 2) {
        throw PDUFormatException("Invalid field count.", Reassemble(tokens));
    }

    QStringList payload;
    if (tokens.size() > 2) { payload = tokens.mid(2); }
    return PDUPC(tokens[0], tokens[1],  payload);
}
