/*****************************************************************************
**
** Copyright (C) 2016 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 31/08/2016.
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
#include "MakeProjectCurrent.h"

#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include <QStringBuilder>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace commands {

    using namespace boost::range;

    /**
     * @brief MakeProjectCurrent::MakeProjectCurrent
     * @param projectName
     * @param model
     */
    MakeProjectCurrent::MakeProjectCurrent(const QString &projectName,
                                           const models::SharedApplicationModel &model)
        : BaseCommand("Activate project -- " % projectName)
        , m_AppModel(model)
        , m_CurrentProjectName(projectName)
    {
    }

    /**
     * @brief MakeProjectCurrent::undo
     */
    void MakeProjectCurrent::undo()
    {
        sanityCheck();

        for_each(m_CurrentGraphicItems, [](auto &&i) { G_ASSERT(i->scene())->removeItem(i); });
        G_ASSERT(m_AppModel->setCurrentProject(m_PreviousProjectName));
        for_each(m_PreviousGraphicItems, [](auto &&i) { G_ASSERT(i->scene())->addItem(i); });
    }

    /**
     * @brief MakeProjectCurrent::redo
     */
    void MakeProjectCurrent::redo()
    {
        if (!m_Done)
        {
            if (auto p = m_AppModel->currentProject())
            {
                m_PreviousProjectName = p->name();
                m_PreviousGraphicItems = p->database()->graphicsItems();
            }

            if (auto p = m_AppModel->project(m_CurrentProjectName))
                m_CurrentGraphicItems = p->database()->graphicsItems();

            m_Done = true;
        }

        sanityCheck();

        for_each(m_PreviousGraphicItems, [](auto &&i) { G_ASSERT(i->scene())->removeItem(i); });
        G_ASSERT(m_AppModel->setCurrentProject(m_CurrentProjectName));
        for_each(m_CurrentGraphicItems, [](auto &&i) { G_ASSERT(i->scene())->addItem(i); });
    }

    /**
     * @brief MakeProjectCurrent::sanityCheck
     */
    void MakeProjectCurrent::sanityCheck()
    {
        if (auto p = m_AppModel->project(m_CurrentProjectName))
            Q_ASSERT_X(equal(p->database()->graphicsItems(), m_CurrentGraphicItems),
                       Q_FUNC_INFO, "Current project is in the inconsistent state.");

        if (auto p = m_AppModel->project(m_PreviousProjectName))
            Q_ASSERT_X(equal(p->database()->graphicsItems(), m_PreviousGraphicItems),
                       Q_FUNC_INFO, "Previous project is in the inconsistent state.");
    }

} // namespace commands
