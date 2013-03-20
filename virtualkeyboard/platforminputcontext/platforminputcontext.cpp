/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "platforminputcontext.h"
#include "declarativeinputcontext.h"

#include <QWindow>
#include <QInputMethod>
#include <QGuiApplication>
#include <qdebug.h>

PlatformInputContext::PlatformInputContext() :
    m_declarativeContext(0), m_animating(false), m_visible(false),
    m_focusObject(0), m_locale("en_GB"), m_inputDirection(Qt::LeftToRight)
{
}

PlatformInputContext::~PlatformInputContext()
{
}

bool PlatformInputContext::isValid() const
{
    return true;
}

bool PlatformInputContext::isAnimating() const
{
    return m_animating;
}

bool PlatformInputContext::isInputPanelVisible() const
{
    return m_visible;
}

QRectF PlatformInputContext::keyboardRect() const
{
    return m_keyboardRect;
}

QLocale PlatformInputContext::locale() const
{
    return m_locale;
}

Qt::LayoutDirection PlatformInputContext::inputDirection() const
{
    return m_inputDirection;
}

void PlatformInputContext::reset()
{
    if (m_declarativeContext)
        m_declarativeContext->commit();
}

void PlatformInputContext::update(Qt::InputMethodQueries queries)
{
    if (m_declarativeContext)
        m_declarativeContext->update(queries);
}

void PlatformInputContext::showInputPanel() {
    if (!m_visible) {
        m_visible = true;
        if (m_declarativeContext) {
            m_declarativeContext->setInputMethodVisibleProperty(true);
        }
        emitInputPanelVisibleChanged();
    }
}

void PlatformInputContext::hideInputPanel() {
    if (m_visible) {
        m_visible = false;
        if (m_declarativeContext) {
            m_declarativeContext->setInputMethodVisibleProperty(false);
        }
        emitInputPanelVisibleChanged();
    }
}

void PlatformInputContext::setFocusObject(QObject *object)
{
    m_focusObject = object;
    bool enabled = false;
    if (m_focusObject) {
        QInputMethodQueryEvent event(Qt::ImEnabled);
        sendEvent(&event);
        enabled = event.value(Qt::ImEnabled).toBool();
    }
    if (m_declarativeContext) {
        bool focus = (object != 0 && enabled);
        m_declarativeContext->setFocus(focus);
        if (focus)
            m_declarativeContext->update(Qt::ImQueryAll);
        else
            hideInputPanel();
    }
}

void PlatformInputContext::sendEvent(QEvent *event)
{
    if (m_focusObject)
        QGuiApplication::sendEvent(m_focusObject, event);
}

void PlatformInputContext::sendKeyEvent(QKeyEvent *event)
{
    if (qApp && qApp->focusWindow())
        QGuiApplication::sendEvent(qApp->focusWindow(), event);
}

QVariant PlatformInputContext::inputMethodQuery(Qt::InputMethodQuery query)
{
    QInputMethodQueryEvent event(query);
    sendEvent(&event);
    return event.value(query);
}

void PlatformInputContext::setDeclarativeContext(DeclarativeInputContext *context)
{
    m_declarativeContext = context;
}
