/*****************************************************************************
**
** Copyright (C) 2018 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/09/2018.
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

#include "QtHelpers.h"

#include <Commands/CommandsTypes.h>

#include <Project/ProjectTypes.hpp>
#include <Project/Project.h>

namespace Commands {

    class CommandFactory : public QObject
    {
        Q_OBJECT

    public:
        SINGLETON(CommandFactory)

        static const CommandFactory &instance();

        template <class Command, class... CmdArgs>
        std::unique_ptr<Command> makeCmd(CmdArgs&&... args) const
        {
            auto cmd = std::make_unique<Command>(std::forward<CmdArgs>(args)...);

            if (m_CurrentProject) {
                cmd->setProjectModified(m_CurrentProject->isModified());
                G_CONNECT(cmd.get(), &Command::changeProjectStatus,
                          m_CurrentProject.get(), &Projects::Project::setModified);
            }

            return cmd;
        }

    public slots:
        void onCurrentProjectChanged(const Projects::SharedProject &previous,
                                     const Projects::SharedProject &current);

    private: // Methods
        CommandFactory();

    private: // Data
        Projects::SharedProject m_CurrentProject;
    };

    template <class Command, class... CmdArgs>
    std::unique_ptr<Command> make(CmdArgs&&... args)
    {
        return CommandFactory::instance().makeCmd<Command>(std::forward<CmdArgs>(args)...);
    }

} // namespace Commands

