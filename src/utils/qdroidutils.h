#ifndef QDROIDUTILS_H
#define QDROIDUTILS_H

#include <qobject.h>

class Q_DECL_EXPORT QDroidUtils : public QObject
{
    Q_OBJECT
public:
    QDroidUtils(QObject* parent = 0) : QObject(parent)
    {
    }
    ~QDroidUtils()
    {
    }

    //### TBD: make an instance() method, for singleton use from C++ ?
    //e.g. connect(myobj, mysig, QDroidUtils::instance(), slot(rebootSystem());

public Q_SLOTS:
    void rebootSystem();
    void powerOffSystem();

    bool setDisplayBrightness(quint8 value);

    QString getIPAddress();
    QString getHostname();
    bool setHostname(QString hostname);
};

#endif // QDROIDUTILS_H
