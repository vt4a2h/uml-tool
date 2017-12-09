/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 08.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/
#include "HeaderEditorEventFilter.h"

#include <QKeyEvent>
#include <QLineEdit>
#include <QPointer>

namespace Graphics {

    /**
     * @brief HeaderEditorEventFilter::HeaderEditorEventFilter
     * @param le
     * @param parent
     */
    HeaderEditorEventFilter::HeaderEditorEventFilter(QLineEdit & le, QObject *parent)
        : QObject(parent)
        , m_Edit(le)
    {
    }

    /**
     * @brief HeaderEditorEventFilter::handleKeyPressEvent
     * @param event
     */
    void HeaderEditorEventFilter::handleKeyPressEvent(const QEvent *event)
    {
        auto *ev = static_cast<const QKeyEvent *>(event);
        switch (ev->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                if (!m_Edit.text().isEmpty())
                    emit nameChanged(m_Edit.text());
                [[fallthrough]];

            case Qt::Key_Escape:
                m_Edit.hide();

            default: ;
        }
    }

    /**
     * @brief HeaderEditorEventFilter::handleFocusOutEvent
     * @param event
     */
    void HeaderEditorEventFilter::handleFocusOutEvent(const QEvent *event)
    {
        Q_UNUSED(event);
        m_Edit.hide();
    }

    /**
     * @brief HeaderEditorEventFilter::eventFilter
     * @param watched
     * @param event
     * @return
     */
    bool HeaderEditorEventFilter::eventFilter(QObject *watched, QEvent *event)
    {
        Q_ASSERT(watched == &m_Edit);

        switch (event->type()) {
            case QEvent::KeyRelease:
                handleKeyPressEvent(event);
                break;

            case QEvent::FocusOut:
                handleFocusOutEvent(event);
                break;

            default: ;
        }

        return QObject::eventFilter(watched, event);
    }

} // namespace Graphics
