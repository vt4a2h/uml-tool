/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 04 .
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

#include <gtest/gtest.h>

#include <QDateTime>

#include <Models/IMessenger.h>
#include <Models/ModelsTypes.hpp>
#include <Entity/Converters/EnumTextConversionStrategy.hpp>

#include "TestProject.h"

namespace Testing
{

    class TestMessenger : public Models::IMessenger
    {
    public: // IMessenger interface
        void addMessage(Models::MessageType type, const QString &summary,
                        const QString &description) override
        {
            m_Messages.push_back({type, summary, description, QDateTime::currentDateTime()});
        }

        void addMessage(const Models::MessageException &e) override
        {
            addMessage(e.type, e.summary, e.description);
        }

        Models::Messages messages() const override
        {
            return m_Messages;
        }

        uint unreadMessagesCount() const override
        {
            // No one will read these messages!
            return m_Messages.count();
        }

        void clear() override
        {
            m_Messages.clear();
        }

    private:
        Models::Messages m_Messages;
    };

    class SectionalTextConvertion : public ProjectBase
    {
    protected:
        void SetUp() final
        {
            m_Messenger = std::make_shared<TestMessenger>();

            m_EnumConversionStrategy->registerMessenger(m_Messenger);
            m_EnumConversionStrategy->registerTypeSearcher(m_GlobalDb);
            m_EnumConversionStrategy->registerTypeSearcher(m_ProjectDb);
        }

    protected:
        Models::SharedMessenger m_Messenger;

        Entity::Converters::SharedConversionStrategy m_EnumConversionStrategy =
            std::make_shared<Entity::Converters::EnumTextConversionStrategy>();
    };

} // namespace Testing
