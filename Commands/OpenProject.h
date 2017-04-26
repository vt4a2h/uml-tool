/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23.
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

#include <models/models_types.hpp>
#include <gui/graphics/GraphicsTypes.h>
#include <project/ProjectTypes.hpp>

#include "BaseCommand.h"
#include "CommandsTypes.h"

QT_BEGIN_NAMESPACE
class QMainWindow;
class QMenu;
QT_END_NAMESPACE

namespace Commands
{

    /**
     * @brief The OpenProject class
     */
    class OpenProject : public BaseCommand
    {
        Q_OBJECT

    public:
        OpenProject(const QString &name, const QString &path,
                    const models::SharedApplicationModel &appModel,
                    const Commands::SharedCommandStack &stack, const graphics::ScenePtr &scene,
                    QMainWindow &mv, QMenu &rp, QUndoCommand *parent = nullptr);

    public: // Types
        using ProjectAdder  = std::function<void(const QString& /*Project name*/)>;
        using MenuRebuilder = std::function<void(QMenu& /*Recent projects menu*/)>;

    public: // QUndoCommand overrides
        void undo()    override;
        void redo()    override;

    public: // BaseCommand overrides
        bool suppressDialogs() const;
        void setSuppressDialogs(bool suppressDialogs);

    signals:
        void recentProjectAdded(const QString &);
        void recentProjectRemoved(const QString &);

    private:
        QString                        m_ProjectPath;
        models::SharedApplicationModel m_AppModel;
        Commands::SharedCommandStack   m_CommandsStack;
        graphics::ScenePtr             m_Scene;
        QMainWindow                    &m_MainWindow;
        QMenu                          &m_RecentProjectsMenu;

        SharedCommand m_MakeProjectCurrentCmd;
        Projects::SharedProject m_Project;

        bool m_SuppressDialogs;
        bool m_commandFailed;
        bool m_UpdateRecentProjectsMenu;
    };

} // Commands
