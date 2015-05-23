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
#include <entity/enum.h>
#include <entity/union.h>
#include <entity/class.h>
#include <entity/templateclass.h>
#include <entity/extendedtype.h>

#include <models/applicationmodel.h>

#include <project/project.h>

#include <db/projectdatabase.h>

#include <utility/helpfunctions.h>

#include <commands/renameentity.h>
#include <commands/movetypetootherscope.h>

#include "enums.h"

namespace gui {

    namespace {
        const QVector< QPair< QString, size_t > > names =
            {
                { EditEntityDialog::tr( "Type" ), entity::Type::staticHashType() },
                { EditEntityDialog::tr( "Enum" ), entity::Enum::staticHashType() },
                { EditEntityDialog::tr( "Union" ), entity::Union::staticHashType() },
                { EditEntityDialog::tr( "Class" ), entity::Class::staticHashType() },
                { EditEntityDialog::tr( "Template class" ), entity::TemplateClass::staticHashType() },
                { EditEntityDialog::tr( "Extended type" ), entity::ExtendedType::staticHashType() }
            };
    }

    /**
     * @brief EditEntityDialog::EditEntityDialog
     * @param parent
     */
    EditEntityDialog::EditEntityDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::EditEntityDialog)
    {
        ui->setupUi(this);

        for(auto &&pair : names)
            ui->cbType->addItem(pair.first, QVariant::fromValue(pair.second));

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
     * @brief EditEntityDialog::setData
     * @param model
     * @param id
     */
    void EditEntityDialog::setData(const models::SharedApplicationModal &model, const entity::SharedType &type)
    {
        Q_ASSERT(model);
        Q_ASSERT(type);

        m_ApplicationModel = model;
        m_Project = m_ApplicationModel->currentProject();
        m_Scope = m_Project->database()->getScope(type->scopeId());
        m_Type = type;
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
        auto &&stack = m_Project->commandsStack();
        stack->beginMacro(tr("Edit entity dialog changes"));

        // Check name
        QString newName = ui->leName->text();
        if (m_Type->name() != newName)
            stack->push(std::make_unique<commands::RenameEntity>(m_Type, newName).release());

        // Check scope
        auto scope = ui->cbScopes->currentData().value<entity::SharedScope>();
        Q_ASSERT(scope);
        if (scope->id() != m_Type->scopeId())
        {
            auto db = m_Project->database();

            auto srcScope = db->getScope(m_Type->scopeId());
            Q_ASSERT(srcScope);

            auto dstScope = db->getScope(scope->id());
            Q_ASSERT(dstScope);

            Q_ASSERT(m_ApplicationModel);
            auto cmd = std::make_unique<commands::MoveTypeToOtherScope>(m_Type, m_ApplicationModel, srcScope, dstScope);
            stack->push(cmd.release());
        }

        stack->endMacro();

        clear();
        accept();
    }

    /**
     * @brief EditEntityDialog::onRejected
     */
    void EditEntityDialog::onRejected()
    {
        clear();
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
            if (m_Type->hashType() == ui->cbType->itemData(i).value<size_t>()) {
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
