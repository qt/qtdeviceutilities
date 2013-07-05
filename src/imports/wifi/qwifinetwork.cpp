#include "qwifinetwork.h"

QWifiNetwork::QWifiNetwork()
{
}

void QWifiNetwork::setSignalStrength(int strength)
{
    if (m_signalStrength == strength)
        return;
    m_signalStrength = strength;
    emit signalStrengthChanged(m_signalStrength);
}
