/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 26/02/2015.
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

#include <QDialog>

namespace GUI {

    namespace Ui {
        class AddScope;
    }

    class AddScope : public QDialog
    {
        Q_OBJECT

    public:
        Q_DISABLE_COPY(AddScope)

        explicit AddScope(QWidget *parent = 0);
        ~AddScope();

        void setProjectName(const QString &name = "");
        void addExistingScopesNames(const QStringList &scopes);

        QString scopeName();

    public slots:
        void onCreateScope();

    private:
        void showEvent(QShowEvent * ev);
        void onTextChanged(const QString &text);

        QScopedPointer<Ui::AddScope> ui;
        QString m_ScopeName;
        QStringList m_ExistingScopes;
    };

} // namespace gui
