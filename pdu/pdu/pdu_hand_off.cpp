

#include "pdu_hand_off.h"

PDUHandOff::PDUHandOff() : PDUBase() {}

PDUHandOff::PDUHandOff(QString callsign,QString target,QString Object) : PDUBase(callsign, target)
{
    this->Object = Object;
}

QStringList PDUHandOff::toTokens() const
{
    QStringList tokens;
    tokens.append(From);
    tokens.append(To);
    tokens.append(Object);
    return tokens;
}

PDUHandOff PDUHandOff::fromTokens(const QStringList &tokens)
{
    if(tokens.size() < 3) {
        throw PDUFormatException("Invalid field count.", Reassemble(tokens));
    }

    return PDUHandOff(tokens[0], tokens[1], tokens[2]);
}
