/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 1)
#include "SurfaceTexture_4_1.h"
#else
#include "SurfaceTexture_4_0.h"
#endif
