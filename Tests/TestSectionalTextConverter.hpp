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

#include <Models/SectionalTextConverter.hpp>
#include <Models/IMessenger.h>
#include <Models/ModelsTypes.hpp>

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

    class SectionalTextConverter : public ProjectBase
    {
    protected:

        void SetUp()
        {
            m_Messenger = std::make_shared<TestMessenger>();

            m_Converter = std::make_shared<Models::SectionalTextConverter>(m_Messenger);
            m_Converter->registerTypeSearcher(m_GlobalDb);
            m_Converter->registerTypeSearcher(m_ProjectDb);
        }

    protected:
        Models::SharedMessenger m_Messenger;
        Models::SharedSectionalTextConverter m_Converter;
    };

} // namespace Testing
