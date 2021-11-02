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
#include <QImage>
#include <QPainter>
#include <QPainterPath>

#include "qtbuttonimageprovider.h"

QtButtonImageProvider::QtButtonImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap QtButtonImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
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
    const qreal borderPenWidth = 2;
    QPen borderPen(QBrush(borderColor), borderPenWidth);
    borderPen.setJoinStyle(Qt::MiterJoin);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(borderPen);
    painter.setBrush(fillColor);

    QPainterPath path;
    qreal top = borderPenWidth - 1;
    qreal left = borderPenWidth - 1;
    qreal bottom = pixmap.height() - borderPenWidth;
    qreal right = pixmap.width() - borderPenWidth;
    path.moveTo(left + cutSize, top);
    path.lineTo(right, top);
    path.lineTo(right, bottom - cutSize);
    path.lineTo(right - cutSize, bottom);
    path.lineTo(left, bottom);
    path.lineTo(left, top + cutSize);
    path.lineTo(left + cutSize, top);
    painter.drawPath(path);

    return pixmap;
}
