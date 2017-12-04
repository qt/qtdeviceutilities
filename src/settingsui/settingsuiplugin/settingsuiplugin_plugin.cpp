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
#include "settingsuiplugin_plugin.h"
#include <qqml.h>

/*!
    \qmlmodule QtDeviceUtilities.SettingsUI 1.0
    \title Qt Device Utilities: Settings UI
    \ingroup qtee-qmlmodules
    \brief Provides a system-wide settings UI.

    The \c DeviceUtilities.SettingsUI module provides a single QML type,
    SettingsUI, that allows applications to integrate a standard, system-wide
    settings view into their user interface.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.SettingsUI 1.0
    \endcode

    \section1 QML Types
*/

/*!
    \qmltype SettingsUI
    \inqmlmodule QtDeviceUtilities.SettingsUI
    \brief Provides a QML type for displaying a system-wide settings UI.

    With the SettingsUI type, applications can easily integrate a standard,
    system-wide settings view into their user interface.

    For example:

    \qml
    import QtQuick 2.6
    import QtDeviceUtilities.SettingsUI 1.0

    SettingsUI {
        id: settingsUI
        anchors.fill: parent
    }
    \endqml

    There is no need to import the various \c {DeviceUtilities.*} modules and
    to declare a custom settings UI, unless specifically required.
*/

QT_BEGIN_NAMESPACE

void SettingsuipluginPlugin::registerTypes(const char *uri)
{
    // @uri QtDeviceUtilities.SettingsUI
    Q_ASSERT(uri == QLatin1String("QtDeviceUtilities.SettingsUI"));
    qmlRegisterType(QUrl("qrc:/pluginMain.qml"), uri, 1, 0, "SettingsUI");
}

QT_END_NAMESPACE
