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

    HeaderEditorEventFilter::HeaderEditorEventFilter(QLineEdit & le, QObject *parent)
        : QObject(parent)
        , m_Edit(le)
    {
    }

    bool HeaderEditorEventFilter::eventFilter(QObject *watched, QEvent *event)
    {
        Q_ASSERT(watched == &m_Edit);

        if (event->type() == QEvent::KeyRelease) {
            auto * ke = static_cast<QKeyEvent *>(event);
            switch (ke->key()) {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    emit nameChanged(m_Edit.text());
                    [[fallthrough]];
                case Qt::Key_Escape:
                    m_Edit.hide();

                default: ;
            }
        }

        return QObject::eventFilter(watched, event);
    }

} // namespace Graphics
