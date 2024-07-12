// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QTBUTTONIMAGEPROVIDER_H
#define QTBUTTONIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QPixmap>

class QtButtonImageProvider : public QQuickImageProvider
{
public:
    QtButtonImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    void setCutSize(int size) { m_cutSize = size; }
private:
    int m_cutSize;
};

#endif // QTBUTTONIMAGEPROVIDER_H
