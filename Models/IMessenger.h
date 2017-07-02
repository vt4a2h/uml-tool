/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 30.
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

#include <QString>
#include <QDateTime>

#include "ModelsTypes.hpp"

namespace Models {

    /**
     * @brief The MessageType enum
     */
    enum class MessageType
    {
        Information,
        Warning,
        Error,
    };

    /**
     * @brief The Message struct
     */
    struct Message
    {
        MessageType type;
        QString     summary;
        QString     description;
        QDateTime   date;
    };

    /// Interface for messaging
    class IMessenger
    {
    public:
        virtual void addMessage(MessageType type, const QString &summary, const QString &description) = 0;
        virtual Messages messages() const = 0;
        virtual uint unreadMessagesCount() const = 0;
    };

} // namespace Models
