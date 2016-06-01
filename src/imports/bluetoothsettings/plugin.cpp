/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <qcoreapplication.h>
#include <QQmlEngine>
#include <QQmlContext>

#include "bluetoothdevice.h"
#include "discoverymodel.h"

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

class BluetoothSettingsQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtDeviceUtilities.BluetoothSettings"));
        qmlRegisterUncreatableType<BtDeviceItem>(uri, 1, 0, "BtDeviceItem", "Cannot be instantiated directly.");
        qmlRegisterSingletonType<BluetoothDevice>(uri, 1, 0, "BtDevice", &instance<BluetoothDevice>);
    }
};

#include "plugin.moc"
