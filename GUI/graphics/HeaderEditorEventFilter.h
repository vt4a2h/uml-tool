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
#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

namespace Graphics {

    class HeaderEditorEventFilter : public QObject
    {
        Q_OBJECT

    public:
        explicit HeaderEditorEventFilter(QLineEdit & le, QObject *parent = nullptr);

        bool eventFilter(QObject *watched, QEvent *event) override;

    signals:
        void nameChanged(const QString &);

    private:
        QLineEdit & m_Edit;
    };

} // namespace Graphics
