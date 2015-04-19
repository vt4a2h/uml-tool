/*****************************************************************************
**
** Copyright (C) 2015 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 18/03/2015.
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
#include "editentitydialog.h"
#include "ui_editentitydialog.h"

namespace gui {

    /**
     * @brief EditEntityDialog::EditEntityDialog
     * @param parent
     */
    EditEntityDialog::EditEntityDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::EditEntityDialog)
    {
        ui->setupUi(this);
    }

    /**
     * @brief EditEntityDialog::~EditEntityDialog
     */
    EditEntityDialog::~EditEntityDialog()
    {
    }

    /**
     * @brief EditEntityDialog::type
     * @return
     */
    entity::SharedType EditEntityDialog::type() const
    {
        return m_Type;
    }

    /**
     * @brief EditEntityDialog::setType
     * @param type
     */
    void EditEntityDialog::setType(const entity::SharedType &type)
    {
        m_Type = type;
    }

    /**
     * @brief EditEntityDialog::scope
     * @return
     */
    entity::SharedScope EditEntityDialog::scope() const
    {
        return m_Scope;
    }

    /**
     * @brief EditEntityDialog::setScope
     * @param scope
     */
    void EditEntityDialog::setScope(const entity::SharedScope &scope)
    {
        m_Scope = scope;
    }

    /**
     * @brief EditEntityDialog::project
     * @return
     */
    project::SharedProject EditEntityDialog::project() const
    {
        return m_Project;
    }

    /**
     * @brief EditEntityDialog::setProject
     * @param project
     */
    void EditEntityDialog::setProject(const project::SharedProject &project)
    {
        m_Project = project;
    }


} // namespace gui
