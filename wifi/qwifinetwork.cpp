#include "qwifinetwork.h"

QWifiNetwork::QWifiNetwork()
{
}

void QWifiNetwork::setSignalStrength(int strenght)
{
    if (m_signalStrength == strenght)
        return;
    m_signalStrength = strenght;
    emit signalStrengthChanged(m_signalStrength);
}
