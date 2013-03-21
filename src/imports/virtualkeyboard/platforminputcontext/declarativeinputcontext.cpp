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

#include <QEvent>
#include <QDebug>
#include <QQuickItem>
#include <QTextFormat>
#include <QGuiApplication>
#include <QLocale>

DeclarativeInputContext::DeclarativeInputContext(PlatformInputContext *parent) :
    QObject(parent), m_inputContext(parent), m_inputMethodVisible(false), m_focus(false),
    m_shift(false), m_capsLock(false), m_cursorPosition(0), m_inputMethodHints(0)
{
    if (m_inputContext)
        m_inputContext->setDeclarativeContext(this);
}

DeclarativeInputContext::~DeclarativeInputContext()
{
    if (m_inputContext)
        m_inputContext->setDeclarativeContext(0);
    m_characterKeys.clear();
}

bool DeclarativeInputContext::inputMethodVisible() const
{
    return m_inputMethodVisible;
}

void DeclarativeInputContext::setInputMethodVisibleProperty(bool visible)
{
    if (m_inputMethodVisible != visible) {
        m_inputMethodVisible = visible;
        emit inputMethodVisibleChanged();
    }
}

bool DeclarativeInputContext::focus() const
{
    return m_focus;
}

void DeclarativeInputContext::setFocus(bool enable)
{
    if (m_focus != enable) {
        m_focus = enable;
        emit focusChanged();
    }
    emit focusEditorChanged();
}

bool DeclarativeInputContext::shift() const
{
    return m_shift;
}

void DeclarativeInputContext::setShift(bool enable)
{
    if (m_shift != enable) {
        m_shift = enable;
        emit shiftChanged();
    }
}

bool DeclarativeInputContext::capsLock() const
{
    return m_capsLock;
}

void DeclarativeInputContext::setCapsLock(bool enable)
{
    if (m_capsLock != enable) {
        m_capsLock = enable;
        emit capsLockChanged();
    }
}

int DeclarativeInputContext::cursorPosition() const
{
    return m_cursorPosition;
}

Qt::InputMethodHints DeclarativeInputContext::inputMethodHints() const
{
    return m_inputMethodHints;
}

QString DeclarativeInputContext::preeditText() const
{
    return m_preeditText;
}

void DeclarativeInputContext::setPreeditText(const QString &text)
{
    if (text != m_preeditText)
        sendPreedit(text);
}

QString DeclarativeInputContext::surroundingText() const
{
    return m_surroundingText;
}

QString DeclarativeInputContext::selectedText() const
{
    return m_selectedText;
}

QRectF DeclarativeInputContext::cursorRectangle() const
{
    return m_cursorRectangle;
}

void DeclarativeInputContext::sendKeyClick(int key, const QString &text, int modifiers)
{
    if (m_focus && m_inputContext) {
        QKeyEvent pressEvent(QEvent::KeyPress, key, Qt::KeyboardModifiers(modifiers), text);
        QKeyEvent releaseEvent(QEvent::KeyRelease, key, Qt::KeyboardModifiers(modifiers), text);

        m_inputContext->sendKeyEvent(&pressEvent);
        m_inputContext->sendKeyEvent(&releaseEvent);
    }
}

void DeclarativeInputContext::sendPreedit(const QString &text, int cursor)
{
    const QString preedit = m_preeditText;
    m_preeditText = text;

    if (m_inputContext) {
        QList<QInputMethodEvent::Attribute> attributes;

        if (cursor >= 0 && cursor <= text.length()) {
            attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::Cursor, cursor, text.length(), QVariant()));
        } else {
            cursor = text.length();
        }

        QTextCharFormat textFormat;
        textFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, text.length(), textFormat));

        QInputMethodEvent event(text, attributes);
        m_inputContext->sendEvent(&event);
    }

    if (m_preeditText != preedit)
        emit preeditTextChanged();
}

void DeclarativeInputContext::commit()
{
    commit(m_preeditText);
}

void DeclarativeInputContext::commit(const QString &text, int replacementStart, int replacementLength)
{
    bool preeditChanged = !m_preeditText.isEmpty();
    m_preeditText.clear();

    if (m_inputContext) {
        QInputMethodEvent inputEvent;
        inputEvent.setCommitString(text, replacementStart, replacementLength);
        m_inputContext->sendEvent(&inputEvent);
    }

    if (preeditChanged)
        emit preeditTextChanged();
}

void DeclarativeInputContext::clear()
{
    bool preeditChanged = !m_preeditText.isEmpty();
    m_preeditText.clear();

    QInputMethodEvent event;
    m_inputContext->sendEvent(&event);

    if (preeditChanged)
        emit preeditTextChanged();
}

void DeclarativeInputContext::setAnimating(bool animating)
{
     if (m_inputContext && m_inputContext->m_animating != animating) {
         m_inputContext->m_animating = animating;
         m_inputContext->emitAnimatingChanged();
     }
}

void DeclarativeInputContext::setKeyboardRectangle(QRectF rectangle)
{
    if (m_inputContext && m_inputContext->m_keyboardRect != rectangle) {
        m_inputContext->m_keyboardRect = rectangle;
        m_inputContext->emitKeyboardRectChanged();
    }
}

QRectF DeclarativeInputContext::keyboardRectangle() const
{
    return m_inputContext->m_keyboardRect;
}

void DeclarativeInputContext::setLocale(QString name)
{
    if (m_inputContext && m_inputContext->m_locale.name() != name) {
        m_inputContext->m_locale = QLocale(name);
        m_inputContext->emitLocaleChanged();
        if (m_inputContext->m_inputDirection != m_inputContext->m_locale.textDirection()) {
            m_inputContext->m_inputDirection = m_inputContext->m_locale.textDirection();
            m_inputContext->emitInputDirectionChanged(m_inputContext->m_inputDirection);
        }
    }
}

void DeclarativeInputContext::update(Qt::InputMethodQueries queries)
{
    Q_UNUSED(queries);

    // fetch
    Qt::InputMethodHints inputMethodHints = Qt::InputMethodHints(m_inputContext->inputMethodQuery(Qt::ImHints).toInt());
    int cursorPosition = m_inputContext->inputMethodQuery(Qt::ImCursorPosition).toInt();
    QRectF cursorRectangle = qApp->inputMethod()->cursorRectangle();
    QString surroundingText = m_inputContext->inputMethodQuery(Qt::ImSurroundingText).toString();
    QString selectedText = m_inputContext->inputMethodQuery(Qt::ImCurrentSelection).toString();

    // check against changes
    bool newInputMethodHints = inputMethodHints != m_inputMethodHints;
    bool newSurroundingText = surroundingText != m_surroundingText;
    bool newSelectedTextChange = selectedText != m_selectedText;
    bool newCursorPosition = cursorPosition != m_cursorPosition;
    bool newCursorRectangle = cursorRectangle != m_cursorRectangle;

    // update
    m_inputMethodHints = inputMethodHints;
    m_surroundingText = surroundingText;
    m_selectedText = selectedText;
    m_cursorPosition = cursorPosition;
    m_cursorRectangle = cursorRectangle;

    // notify
    if (newInputMethodHints)
        emit inputMethodHintsChanged();
    if (newSurroundingText)
        emit surroundingTextChanged();
    if (newSelectedTextChange)
        emit selectedTextChanged();
    if (newCursorPosition)
        emit cursorPositionChanged();
    if (newCursorRectangle)
        emit cursorRectangleChanged();
}

void DeclarativeInputContext::registerCharacterKey(QQuickItem *key)
{
    m_characterKeys.append(key);
}

void DeclarativeInputContext::unregisterCharacterKey(QQuickItem *key)
{
    m_characterKeys.removeOne(key);
}

void DeclarativeInputContext::handleUppercasing(bool uppercased)
{
    foreach(QQuickItem* key, m_characterKeys)
        key->setProperty("uppercased", uppercased);
}
