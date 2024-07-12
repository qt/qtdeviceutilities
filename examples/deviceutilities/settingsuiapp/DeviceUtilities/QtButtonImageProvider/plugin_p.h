// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QTBUTTONIMAGEPROVIDERQMLPLUGIN_H
#define QTBUTTONIMAGEPROVIDERQMLPLUGIN_H

#include "qtbuttonimageprovider.h"
#include <qqmlextensionplugin.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

class QtButtonImageProviderQmlPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void initializeEngine(QQmlEngine *engine, const char *uri) final
    {
        Q_UNUSED(uri);
        engine->addImageProvider("QtButton", new QtButtonImageProvider);
    }
};

QT_END_NAMESPACE
#endif //QTBUTTONIMAGEPROVIDERQMLPLUGIN_H
