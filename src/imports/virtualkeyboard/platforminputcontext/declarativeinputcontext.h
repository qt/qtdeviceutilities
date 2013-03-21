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

#ifndef DECLARATIVEINPUTCONTEXT_H
#define DECLARATIVEINPUTCONTEXT_H

#include <QObject>
#include <QRectF>
#include <QFont>
#include <QtQml/qqml.h>

class PlatformInputContext;
class QQuickItem;

class DeclarativeInputContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool inputMethodVisible READ inputMethodVisible NOTIFY inputMethodVisibleChanged)
    Q_PROPERTY(bool focus READ focus NOTIFY focusChanged)
    Q_PROPERTY(bool shift READ shift WRITE setShift NOTIFY shiftChanged)
    Q_PROPERTY(bool capsLock READ capsLock WRITE setCapsLock NOTIFY capsLockChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(Qt::InputMethodHints inputMethodHints READ inputMethodHints NOTIFY inputMethodHintsChanged)
    Q_PROPERTY(QString preeditText READ preeditText WRITE setPreeditText NOTIFY preeditTextChanged)
    Q_PROPERTY(QString surroundingText READ surroundingText NOTIFY surroundingTextChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY selectedTextChanged)
    Q_PROPERTY(QRectF cursorRectangle READ cursorRectangle NOTIFY cursorRectangleChanged)
    Q_PROPERTY(QRectF keyboardRectangle READ keyboardRectangle WRITE setKeyboardRectangle NOTIFY keyboardRectangleChanged)

public:
    explicit DeclarativeInputContext(PlatformInputContext *parent = 0);
    ~DeclarativeInputContext();

    bool inputMethodVisible() const;
    void setInputMethodVisibleProperty(bool visible);

    bool focus() const;
    void setFocus(bool enable);

    bool shift() const;
    void setShift(bool enable);

    bool capsLock() const;
    void setCapsLock(bool enable);

    int cursorPosition() const;

    Qt::InputMethodHints inputMethodHints() const;

    QString preeditText() const;
    void setPreeditText(const QString &text);

    QString surroundingText() const;

    QString selectedText() const;

    QRectF cursorRectangle() const;

    QRectF keyboardRectangle() const;
    Q_INVOKABLE void setKeyboardRectangle(QRectF rectangle);

    Q_INVOKABLE void sendKeyClick(int key, const QString &text, int modifiers = 0);
    Q_INVOKABLE void sendPreedit(const QString &text, int cursor = -1);

    Q_INVOKABLE void commit();
    Q_INVOKABLE void commit(const QString &text, int replacementStart = 0, int replacementEnd = 0);

    Q_INVOKABLE void clear();

    Q_INVOKABLE void setAnimating(bool animating);
    Q_INVOKABLE void setLocale(QString name);

    void update(Qt::InputMethodQueries queries);

    Q_INVOKABLE void registerCharacterKey(QQuickItem * key);
    Q_INVOKABLE void unregisterCharacterKey(QQuickItem * key);
    Q_INVOKABLE void handleUppercasing(bool uppercased);

signals:
    void inputMethodVisibleChanged();
    void focusChanged();
    void preeditTextChanged();
    void surroundingTextChanged();
    void selectedTextChanged();
    void cursorPositionChanged();
    void inputMethodHintsChanged();
    void focusEditorChanged();
    void shiftChanged();
    void capsLockChanged();
    void cursorRectangleChanged();
    void keyboardRectangleChanged();

private:
    void updateInputMethodValues();

    PlatformInputContext *m_inputContext;
    bool m_inputMethodVisible;
    bool m_focus;
    bool m_shift;
    bool m_capsLock;
    int m_cursorPosition;
    Qt::InputMethodHints m_inputMethodHints;
    QString m_preeditText;
    QString m_surroundingText;
    QString m_selectedText;
    QRectF m_cursorRectangle;
    QList<QQuickItem *> m_characterKeys;
};

QML_DECLARE_TYPE(DeclarativeInputContext)

#endif
