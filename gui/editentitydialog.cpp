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

#include <entity/type.h>
#include <entity/scope.h>

#include <project/project.h>

#include <db/projectdatabase.h>

#include <utility/helpfunctions.h>

#include <commands/renameentity.h>
#include <commands/undostack.h>

#include <enums.h>

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

        for(int i = 0; i < int(entity::UserType::Count); ++i)
            ui->cbType->addItem(utility::userTypeToString(entity::UserType(i)),
                                QVariant::fromValue(entity::UserType(i)));

        connect(ui->pbAccept, &QPushButton::clicked, this, &EditEntityDialog::onAccepted);
        connect(ui->pbReject, &QPushButton::clicked, this, &EditEntityDialog::onRejected);
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

    /**
     * @brief EditEntityDialog::showEvent
     * @param ev
     */
    void EditEntityDialog::showEvent(QShowEvent *ev)
    {
        init();

        QDialog::showEvent(ev);
    }

    /**
     * @brief EditEntityDialog::closeEvent
     * @param ev
     */
    void EditEntityDialog::closeEvent(QCloseEvent *ev)
    {
        clear();

        QDialog::closeEvent(ev);
    }

    /**
     * @brief EditEntityDialog::onAccepted
     */
    void EditEntityDialog::onAccepted()
    {
        // TODO: need to implement auto repeint and auto model update
        auto &&stack = m_Project->commandsStack();
        stack->beginMacro(tr("Edit entity dialog changes"));

        // Check name
        QString newName = ui->leName->text();
        if (m_Type->name() != newName)
        {
            auto renameCmd = std::make_unique<commands::RenameEntity>(m_Type, newName);
            stack->push(renameCmd.release());
        }

        stack->endMacro();
        accept();
    }

    /**
     * @brief EditEntityDialog::onRejected
     */
    void EditEntityDialog::onRejected()
    {
        reject();
    }

    /**
     * @brief EditEntityDialog::init
     */
    void EditEntityDialog::init()
    {
        Q_ASSERT(m_Type);
        Q_ASSERT(m_Scope);
        Q_ASSERT(m_Project);

        ui->leName->setText(m_Type->name());

        setType();

        auto &&db = m_Project->database();
        for(auto &&scope : db->scopes())
            ui->cbScopes->addItem(scope->name(), QVariant::fromValue(scope));
        setScope();
    }

    /**
     * @brief EditEntityDialog::clear
     */
    void EditEntityDialog::clear()
    {
        ui->leName->clear();
        ui->cbScopes->clear();
    }

    /**
     * @brief EditEntityDialog::setType
     */
    void EditEntityDialog::setType()
    {
        for (int i = 0; i < ui->cbType->count(); ++i) {
            if (m_Type->type() == ui->cbType->itemData(i).value<entity::UserType>()) {
                ui->cbType->setCurrentIndex(i);
                break;
            }
        }
    }

    /**
     * @brief EditEntityDialog::setScope
     */
    void EditEntityDialog::setScope()
    {
        for (int i = 0; i < ui->cbScopes->count(); ++i) {
            if (m_Scope == ui->cbScopes->itemData(i).value<entity::SharedScope>()) {
                ui->cbScopes->setCurrentIndex(i);
                break;
            }
        }
    }

} // namespace gui
