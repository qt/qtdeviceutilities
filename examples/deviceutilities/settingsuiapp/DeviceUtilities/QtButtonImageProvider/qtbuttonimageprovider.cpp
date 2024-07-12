// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
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
