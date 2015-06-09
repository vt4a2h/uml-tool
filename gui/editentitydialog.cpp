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
#include <entity/classmethod.h> // TODO: remove

#include <models/applicationmodel.h>
#include <models/classcomponentsmodel.h>

#include <project/project.h>

#include <db/projectdatabase.h>

#include <utility/helpfunctions.h>

#include <commands/renameentity.h>
#include <commands/movetypetootherscope.h>

#include "classcomponentseditdelegate.h"
#include "signaturemaker.h"
#include "editmethoddialog.h"

#include "enums.h"
#include "constants.cpp"

namespace gui {

    namespace {
        const QVector<QPair<QString, size_t>> names =
            {
                { EditEntityDialog::tr("Type"), entity::Type::staticHashType() },
                { EditEntityDialog::tr("Enum"), entity::Enum::staticHashType() },
                { EditEntityDialog::tr("Union"), entity::Union::staticHashType() },
                { EditEntityDialog::tr("Class"), entity::Class::staticHashType() },
                { EditEntityDialog::tr("Template class"), entity::TemplateClass::staticHashType() },
                { EditEntityDialog::tr("Extended type"), entity::ExtendedType::staticHashType() }
            };

        const QVector<QPair<QString, QString>> componentsNames =
            {
                { EditEntityDialog::tr("Properties"), EditEntityDialog::tr("property") },
                { EditEntityDialog::tr("Methods")   , EditEntityDialog::tr("method")   },
                { EditEntityDialog::tr("Fields")    , EditEntityDialog::tr("field")    },
                { EditEntityDialog::tr("Elements")  , EditEntityDialog::tr("element")  },
            };

        const QMap<size_t, QStringList> componentsPreset =
            {
                { entity::Class::staticHashType()        , { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { entity::TemplateClass::staticHashType(), { EditEntityDialog::tr("Methods"),
                                                             EditEntityDialog::tr("Properties"),
                                                             EditEntityDialog::tr("Fields") } },

                { entity::Union::staticHashType()       , { EditEntityDialog::tr("Fields")   } },
                { entity::Enum::staticHashType()        , { EditEntityDialog::tr("Elements") } },
                { entity::Type::staticHashType()        , {} },
                { entity::ExtendedType::staticHashType(), {} }
            };

        const QVector<QPair<QString, models::DisplayPart>> display =
            {
                { EditEntityDialog::tr("Properties"), models::DisplayPart::Properties },
                { EditEntityDialog::tr("Methods")   , models::DisplayPart::Methods    },
                { EditEntityDialog::tr("Fields")    , models::DisplayPart::Fields     },
                { EditEntityDialog::tr("Elements")  , models::DisplayPart::Elements   },
            };

        enum class ComponentCustomRoles : int {
            Single = 300,
            Display,
        };

        void setButtonLabel(QPushButton *btn, QListWidgetItem *item)
        {
            Q_ASSERT(btn); Q_ASSERT(item);

            btn->setText(
                EditEntityDialog::tr("&Add new %1").arg(item->data(int(ComponentCustomRoles::Single)).toString())
            );
        }

        QListWidgetItem *itemByName(const QString &name, const QListWidget *wgt)
        {
            QListWidgetItem *result = nullptr;
            for (int i = 0; i < wgt->count(); ++i)
            {
                if (wgt->item(i)->text() == name) {
                    result = wgt->item(i);
                    break;
                }
            }

            Q_ASSERT(result);
            return result;
        }

        void configure(const QScopedPointer<Ui::EditEntityDialog> &ui, const entity::SharedType &type)
        {
            for (int i = 0; i < ui->lstMembers->count(); ++i)
                ui->lstMembers->item(i)->setHidden(true);

            for (auto &&name : componentsPreset[type->hashType()])
                itemByName(name, ui->lstMembers)->setHidden(false);
        }
    }

    /**
     * @brief EditEntityDialog::EditEntityDialog
     * @param parent
     */
    EditEntityDialog::EditEntityDialog(QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::EditEntityDialog)
        , m_EditMethodDialog(new EditMethodDialog(this))
        , m_ComponentsModel(std::make_unique<models::ClassComponentsModel>(nullptr))
    {
        ui->setupUi(this);

        for(auto &&pair : names)
            ui->cbType->addItem(pair.first, QVariant::fromValue(pair.second));
        for (auto &&p : componentsNames)
            itemByName(p.first, ui->lstMembers)->setData(int(ComponentCustomRoles::Single), p.second);
        for (auto &&p : display)
            itemByName(p.first, ui->lstMembers)->setData(int(ComponentCustomRoles::Display),
                                                         QVariant::fromValue(p.second));

        ui->lstMembers->setCurrentRow(0);
        ui->viewMembers->setModel(m_ComponentsModel.get());

        auto delegat = new ClassComponentsEditDelegate(this);
        connect(delegat, &ClassComponentsEditDelegate::editButtonClicked,
                this, &EditEntityDialog::onEditComponentClicked);
        connect(delegat, &ClassComponentsEditDelegate::deleteButtonClicked,
                this, &EditEntityDialog::onDeleteComponentClicked);
        ui->viewMembers->setItemDelegateForColumn(models::ClassComponentsModel::Buttons, delegat);
        connect(m_ComponentsModel.get(), &models::ClassComponentsModel::showButtonsForIndex,
                [view = ui->viewMembers](auto index) {
                    Q_ASSERT(index.isValid());
                    view->openPersistentEditor(index);
                });

        QHeaderView * header = ui->viewMembers->horizontalHeader();
        header->setSectionResizeMode(models::ClassComponentsModel::ShortSignature, QHeaderView::Stretch);
        header->setSectionResizeMode(models::ClassComponentsModel::Buttons, QHeaderView::Fixed);
        ui->viewMembers->setColumnWidth(models::ClassComponentsModel::Buttons, delegat->size().width());

        connect(ui->pbAccept, &QPushButton::clicked, this, &EditEntityDialog::onAccepted);
        connect(ui->pbReject, &QPushButton::clicked, this, &EditEntityDialog::onRejected);
        connect(ui->pbCreateScope, &QPushButton::clicked, this, &EditEntityDialog::needNewScope);
        connect(ui->lstMembers, &QListWidget::currentItemChanged, this, &EditEntityDialog::onCurrentItemChange);
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

        // Temporary {
        auto cl = std::static_pointer_cast<entity::Class>(m_Type);

        if (cl->methods().isEmpty()) {
            auto intType = m_Scope->addType("int");
            auto doubleType = m_Scope->addType("double");

            cl->makeMethod("foo")->setReturnTypeId(intType->id());
            cl->makeMethod("bar")->setReturnTypeId(doubleType->id());
            cl->makeMethod("baz");
        }

        if (cl->fields().isEmpty()) {
            cl->addField("f1", "stub");
            cl->addField("f2", "stub");
        }
        // }

        auto signatureMaker(std::make_unique<SignatureMaker>(m_ApplicationModel, m_Project, m_Scope, m_Type));
        m_ComponentsModel->setSignatureMaker(std::move(signatureMaker));
        m_ComponentsModel->setComponents(m_Type);
    }

    /**
     * @brief EditEntityDialog::onScopeAdded
     * @param scope
     */
    void EditEntityDialog::onScopeAdded(const entity::SharedScope &scope)
    {
        ui->cbScopes->addItem(scope->name(), QVariant::fromValue(scope));
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
     * @brief EditEntityDialog::onNewComponentClicked
     */
    void EditEntityDialog::onNewComponentClicked()
    {
        Q_ASSERT(m_Type);
    }

    /**
     * @brief EditEntityDialog::onCurrentItemChange
     * @param current
     * @param previous
     */
    void EditEntityDialog::onCurrentItemChange(QListWidgetItem *current, QListWidgetItem *previous)
    {
        Q_UNUSED(previous);
        setButtonLabel(ui->pbNewComponent, current);
        m_ComponentsModel->setDisplay(current->data(int(ComponentCustomRoles::Display)).value<models::DisplayPart>());
    }

    /**
     * @brief EditEntityDialog::onEditComponentClicked
     * @param index
     */
    void EditEntityDialog::onEditComponentClicked(const QModelIndex &index)
    {
        // TODO: cover other components (fields, enum elements and properties)
        if (m_ComponentsModel->display() == models::DisplayPart::Methods) {
            auto data = index.data(models::ClassComponentsModel::InternalData);

            Q_ASSERT(data.isValid());
            Q_ASSERT(data.canConvert<entity::SharedMethod>());

            m_EditMethodDialog->setCurrentMethod(data.value<entity::SharedMethod>());
            m_EditMethodDialog->show(); // NOTE: maybe use exec to handle result
        }
    }

    /**
     * @brief EditEntityDialog::onDeleteComponentClicked
     * @param index
     */
    void EditEntityDialog::onDeleteComponentClicked(const QModelIndex &index)
    {
        Q_UNUSED(index);
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

        setButtonLabel(ui->pbNewComponent, ui->lstMembers->currentItem());
        configure(ui, m_Type);
    }

    /**
     * @brief EditEntityDialog::clear
     */
    void EditEntityDialog::clear()
    {
        ui->leName->clear();
        ui->cbScopes->clear();
        m_ComponentsModel->clear();
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
