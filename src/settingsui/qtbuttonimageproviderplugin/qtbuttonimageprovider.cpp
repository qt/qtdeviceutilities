/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>

class QtButtonImageProvider : public QQuickImageProvider
{
public:
    QtButtonImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        bool ok = false;

        QStringList params = id.split("/");

        int cutSize = params.at(0).toInt(&ok);

        if (!ok)
            cutSize = 10;

        QColor fillColor;
        QColor borderColor;

        if (params.length() > 1) {
            fillColor = QColor(params.at(1));
        }

        if (params.length() > 2)
            borderColor = QColor(params.at(2));

        if (!fillColor.isValid())
            fillColor = "#00eb00";

        if (!borderColor.isValid())
            borderColor ="white";

        int width = 100;
        int height = 50;

        if (size)
            *size = QSize(requestedSize.width(), requestedSize.height());

        QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                       requestedSize.height() > 0 ? requestedSize.height() : height);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(borderColor);
        painter.setBrush(fillColor);

        QPainterPath path;
        path.moveTo(cutSize,0);
        path.lineTo(pixmap.width(), 0);
        path.lineTo(pixmap.width(), pixmap.height()-cutSize);
        path.lineTo(pixmap.width()-cutSize, pixmap.height());
        path.lineTo(0, pixmap.height());
        path.lineTo(0, cutSize);
        path.lineTo(cutSize, 0);
        path.closeSubpath();
        painter.drawPath(path);

        return pixmap;
    }
};

class ImageProviderExtensionPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri)
    {
        Q_UNUSED(uri)
        Q_ASSERT(uri == QLatin1String("QtDeviceUtilities.QtButtonImageProvider"));
        qmlRegisterType(QUrl("qrc:/QtButton.qml"), uri, 1, 0, "QtButton");
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        QtButtonImageProvider *provider = new QtButtonImageProvider();
        engine->addImageProvider("QtButton", provider);
    }
};

#include "qtbuttonimageprovider.moc"
