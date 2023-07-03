

#include "pdu_hand_off_acknowledge.h"

PDUHandOffAcknowledge::PDUHandOffAcknowledge() : PDUBase() {}

PDUHandOffAcknowledge::PDUHandOffAcknowledge(QString callsign,QString target,QString tCallsign) : PDUBase(callsign, target)
{
    this->tCallsign = tCallsign;
}

QStringList PDUHandOffAcknowledge::toTokens() const
{
    QStringList tokens;
    tokens.append(From);
    tokens.append(To);
    tokens.append(tCallsign);
    return tokens;
}

PDUHandOffAcknowledge PDUHandOffAcknowledge::fromTokens(const QStringList &tokens)
{
    if(tokens.size() < 3) {
        throw PDUFormatException("Invalid field count.", Reassemble(tokens));
    }
    return PDUHandOffAcknowledge(tokens[0], tokens[1], tokens[2]);
}
