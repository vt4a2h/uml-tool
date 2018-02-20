/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 20 .
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
#include <QString>

#include <DB/DBTypes.hpp>
#include <Common/CommonTypes.hpp>

namespace Models
{

    /// Convert basic objects from text and vice versa
    class SectionalTextConverter : public QObject
    {
        Q_OBJECT

    public:
        explicit SectionalTextConverter(QObject *parent = nullptr);

        QString toString(Common::BasicElement const & element) const noexcept;
        QStringList fromString(QString const & s, Common::BasicElement & element) const noexcept;

    public slots:
        void registerTypeSearcher(DB::SharedTypeSearcher const & typeSearcher);
        void unregisterTypeSearcher(DB::SharedTypeSearcher const & typeSearcher);

    private:
        DB::WeakTypeSearchers m_TypeSearchers;
    };

} // namespace Models
