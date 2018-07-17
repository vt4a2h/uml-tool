/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18.
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

#include <functional>

#include <QWidget>

#include <Models/ModelsTypes.hpp>

namespace GUI {

    namespace Ui {
        class Section;
    }

    /// Displays one section with some data (e.g. all fields/methods/properties/etc.)
    class Section : public QWidget
    {
        Q_OBJECT

    public:
        explicit Section(const QString &sectionName, const QString &help, QWidget *parent = nullptr);
        ~Section() override;

        QString sectionName() const;
        void setSectionName(const QString &sectionName);

        QString sectionHelp() const;
        void setSectionHelp(const QString &sectionHelp);

    protected:
        void mouseReleaseEvent(QMouseEvent *event) override;

    private: // Types
        enum State { Opened, Closed };

    private: // Methods
        void setState(State state);

    private:
        QScopedPointer<Ui::Section> m_ui;

        State m_State;
        QString m_SectionName;
        QString m_SectionHelp;
    };


} // namespace GUI
